#include "objects/voxel/Chunk.hpp"

#include <array>
#include <thread>

#include "VoxelData.hpp"
#include "Chunks.hpp"
#include "BuildMesh.hpp"
#include "../ObjectResources.hpp"
#include "../loaders/Polygons.hpp"
#include "../../webgpu/primitives/buffers/AttributeBuffer.hpp"


//        -----<----------------<----------------<----------------
//       |                             ----------<---------<----  |
//       |                            |                         | |
// UNLOADED (Not visible) -> GENERATING_MESH -> MESH_READY -> LOADED (Visible) 
const char CHUNK_INTERNAL_UNLOADED = 0x00;
const char CHUNK_INTERNAL_LOADED = 0x01;
const char CHUNK_INTERNAL_GENERATING_MESH = 0x02;
const char CHUNK_INTERNAL_MESH_READY = 0x03;

struct GPU_CHUNK {
    // Buffer containing mesh to draw chunk
    std::shared_ptr<tinyrender::AttributeBuffer> buffer = nullptr;
    // Uniforms
    std::shared_ptr<ObjectResources> resources = nullptr;

    GPU_CHUNK(Context *c, Scene *s, std::shared_ptr<VoxelMesh> cpu, ivec2 cornerCoordinate, std::shared_ptr<tinyrender::ModelMatrixUniform> globalModelMatrix);

    void onDraw(wgpu::RenderPassEncoder &renderPass, int vertexBufferSlot, int bindGroupSlot);
};

GPU_CHUNK::GPU_CHUNK(Context *c, Scene *s, std::shared_ptr<VoxelMesh> cpu, ivec2 cornerCoordinate, std::shared_ptr<tinyrender::ModelMatrixUniform> globalModelMatrix) {
    if(cpu->size() == 0)
        return; // empty chunk

    // Flatten
    auto *data = reinterpret_cast<float *>(cpu->data()); 
    int n_data = sizeof(VoxelVertexAttribute)/sizeof(float) * cpu->size();
    auto v = std::vector<float>(data, data + n_data);

    buffer = std::make_shared<tinyrender::AttributeBuffer>(c, v, cpu->size());
    resources = std::make_shared<ObjectResources>(c, s, this->buffer, ColoredTriangle);
    resources->modelMatrix->setTranslation(glm::vec3(cornerCoordinate, 0.0));
    resources->globalModelMatrix = globalModelMatrix;
    resources->resetBindGroup(Voxels);
}

void GPU_CHUNK::onDraw(wgpu::RenderPassEncoder &renderPass, int vertexBufferSlot, int bindGroupSlot) {
    if(buffer == nullptr || buffer->getDrawCalls() == 0)
        return;

    renderPass.setVertexBuffer(vertexBufferSlot, buffer->getUnderlyingBuffer(), 0, buffer->getSize());
    renderPass.setBindGroup(bindGroupSlot, resources->bindGroup, 0, nullptr);
    renderPass.draw(buffer->getDrawCalls(), 1, 0, 0);
}


Chunk::Chunk(Context *c, Scene *s, Chunks &chunks, ivec2 chunkCoordinate, std::shared_ptr<tinyrender::ModelMatrixUniform> globalModelMatrix): c(c), s(s), chunks(chunks) {
    this->chunkCoordinate = chunkCoordinate;
    this->cornerCoordinate = glm::ivec2(SIZE_XY, SIZE_XY) * chunkCoordinate;
    this->globalModelMatrix = globalModelMatrix;
}

void Chunk::onDraw(wgpu::RenderPassEncoder &renderPass, int vertexBufferSlot, int bindGroupSlot) {
    if(gpu != nullptr)
        gpu->onDraw(renderPass, vertexBufferSlot, bindGroupSlot);
}

void Chunk::onUpdate() {
    // State machine is handled here, except for when offloaded to buildMeshAsync()
    switch(this->state.load()) {
        case CHUNK_INTERNAL_UNLOADED: {
            if(should_build_mesh) {
                should_build_mesh = false;
                buildMeshAsync();
            }
            break;
        }
        case CHUNK_INTERNAL_MESH_READY: {
            // Upload mesh to GPU and fall through to LOADED state
            this->gpu = std::make_unique<GPU_CHUNK>(c, s, mesh, cornerCoordinate, globalModelMatrix);
            this->state.store(CHUNK_INTERNAL_LOADED);
        }
        case CHUNK_INTERNAL_LOADED: {
            if(should_unload) {
                this->mesh = nullptr;
                this->gpu.reset();
                this->state.store(CHUNK_INTERNAL_UNLOADED);
                should_unload = false;
                should_build_mesh = false;
                break;
            }
            if(should_build_mesh) {
                should_build_mesh = false;
                buildMeshAsync();
            }
            break;
        }
        case CHUNK_INTERNAL_GENERATING_MESH: 
        default:
            break;
    }
}

void Chunk::refreshNeighbours()
{
    auto refresh = [&](ivec2 chunk) {
        if(chunks.chunkTracked(chunk) && chunks.getChunk(chunk)->isVisible())
            chunks.getChunk(chunk)->shouldRefreshMesh();
    };
    refresh(chunkCoordinate + ivec2(0, 1));
    refresh(chunkCoordinate + ivec2(0, -1));
    refresh(chunkCoordinate + ivec2(1, 0));
    refresh(chunkCoordinate + ivec2(-1, 0));
}

void Chunk::buildMeshAsync()
{
    this->state.store(CHUNK_INTERNAL_GENERATING_MESH);
    auto neighbourData = extractBoundaries(chunks, chunkCoordinate); // Extract on main thread 

    // Build the mesh in a new thread
    auto func = [&, neighbourData]{
        this->mesh = buildMeshGridSearch(this->voxels, neighbourData);
        this->state.store(CHUNK_INTERNAL_MESH_READY);
    };
    auto thread = std::thread(func);
    thread.detach();
}

void Chunk::setVisibility(const char state) {
    switch(state) {
        case CHUNK_VISIBLE: {
            if(this->state.load() == CHUNK_INTERNAL_UNLOADED) {
                this->should_build_mesh = true;
                refreshNeighbours();
            }
            return;
        }
        case CHUNK_HIDDEN: {
            if(this->state.load() != CHUNK_INTERNAL_UNLOADED) {
                this->should_unload = true;
                refreshNeighbours();
            }
            return;
        }
    }
    throw std::runtime_error("Invalid state passed to chunk visibility setting");
}

bool Chunk::isVisible() {
    // Visible / intention to be visible
    return (this->state.load() != CHUNK_INTERNAL_UNLOADED || should_build_mesh) && !should_unload;
}

void Chunk::set(ivec3 voxel, char value) {
    this->voxels[idx(voxel)] = value;
    this->should_build_mesh = true;
}

void Chunk::shouldRefreshMesh() {
    if(this->state.load() != CHUNK_INTERNAL_UNLOADED)
        this->should_build_mesh = true;
}

Chunk::~Chunk() = default;