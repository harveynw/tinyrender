#include "Chunk.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten/wasm_worker.h>
#endif

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

    minecraft(voxels, this->cornerCoordinate);

    refreshNeighbours();
}

void Chunk::onDraw(wgpu::RenderPassEncoder &renderPass, int vertexBufferSlot, int bindGroupSlot) {
    if(gpu != nullptr)
        gpu->onDraw(renderPass, vertexBufferSlot, bindGroupSlot);
}

void Chunk::onUpdate(ivec2 cameraChunk) {
    (void) cameraChunk;

    // State machine is handled here, except for when offloaded to buildMeshAsync()
    switch(this->state.load()) {
        case CHUNK_INTERNAL_UNLOADED: {
            if(should_build_mesh) {
                printf("%p: UNLOADED -> should_build_mesh\n", (void*) this);
                should_build_mesh = false;
                this->buildMeshAsync();
            }
            break;
        }
        case CHUNK_INTERNAL_LOADED: {
            if(should_unload) {
                printf("%p: LOADED -> should_unload\n", (void*) this);
                this->mesh = nullptr;
                this->gpu.reset();
                this->state.store(CHUNK_INTERNAL_UNLOADED);
                should_unload = false;
                refreshNeighbours();
                break;
            }
            if(should_build_mesh) {
                printf("%p: LOADED -> should_build_mesh\n", (void*) this);
                should_build_mesh = false;
                this->gpu.reset();
                this->buildMeshAsync();
            }
            if(this->mesh != nullptr && this->gpu == nullptr) {
                // Mesh waiting to be uploaded to GPU 
                printf("%p: LOADED -> mesh found, setting this->gpu \n", (void*) this);
                this->gpu = std::make_unique<GPU_CHUNK>(c, s, mesh, cornerCoordinate, globalModelMatrix);
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

/*
#ifdef __EMSCRIPTEN__
// Emscripten provides a C-API for web workers, which requires a C-style function ptr
#include <map>

namespace {
    struct BuildMeshJob {
        // Input
        std::array<char, N_VOXELS> voxels;
        NeighbourData neighbourData;

        // Output
        std::shared_ptr<VoxelMesh> *mesh;
        std::atomic<char> *state;

        void execute() {
            mesh->reset();
            *mesh = buildMeshGridSearchBoundaries(voxels, neighbourData);
            state->store(CHUNK_INTERNAL_LOADED);
        }
    };
    std::mutex job_mutex;
    std::map<int, BuildMeshJob> jobs;

    void executeBuildMeshJob(int i) {
        BuildMeshJob job;
        {
            const std::lock_guard<std::mutex> lock(job_mutex);
            job = jobs.at(i);
        }
        job.execute();
        {
            const std::lock_guard<std::mutex> lock(job_mutex);
            jobs.erase(i);
        }
    }

    void buildMeshAsyncEmscripten(BuildMeshJob job) {
        // Pass job into thread safe/"worker" safe data structure
        int idx;
        {
            const std::lock_guard<std::mutex> lock(job_mutex);
            for(int i = 0; true; i++) {
                if(jobs.count(i) == 0) {
                    idx = i; 
                    break;
                }
            }
            jobs.at(idx) = job;
        }

        // Start web worker
        emscripten_wasm_worker_t worker = emscripten_malloc_wasm_worker(1024);
        emscripten_wasm_worker_post_function_vi(worker, executeBuildMeshJob, idx);
    }
}
#endif
*/

void Chunk::buildMeshAsync()
{
    printf("%p: buildMeshAsync()\n", (void*) this);
    this->state.store(CHUNK_INTERNAL_GENERATING_MESH);
    auto neighbourData = extractBoundaries(chunks, chunkCoordinate); // Extract on main thread 

    auto func = [&, neighbourData]{
        this->mesh = buildMeshGridSearchBoundaries(this->voxels, neighbourData);
        this->state.store(CHUNK_INTERNAL_LOADED);
    };

    //this->mesh = buildMeshNaive(this->voxels); 
    //this->mesh = buildMeshCullBoundaries(this->voxels, neighbourData);
    //this->mesh = buildMeshGridSearch(this->voxels);
    //neighbourData.print();

    // Build the mesh in a new thread/webworker
    /*
    #ifdef __EMSCRIPTEN__
    BuildMeshJob job = {this->voxels, neighbourData, &this->mesh, &this->state};
    buildMeshAsyncEmscripten(job);
    #else
    #ifdef __EMSCRIPTEN__
        #ifndef __EMSCRIPTEN_PTHREADS__
            #error Need pthreads
        #endif
    #endif
    */
    #ifdef __EMSCRIPTEN__
    func();
    #else
    func();
    //auto thread = std::thread(func);
    //thread.detach();
    #endif
}

void Chunk::setVisibility(const char state) {
    switch(state) {
        case CHUNK_VISIBLE: {
            if(this->state.load() == CHUNK_INTERNAL_UNLOADED)
                this->should_build_mesh = true;
            return;
        }
        case CHUNK_HIDDEN: {
            if(this->state.load() != CHUNK_INTERNAL_UNLOADED)
                this->should_unload = true;
            return;
        }
    }
    throw std::runtime_error("Invalid state passed to chunk visibility setting");
}

bool Chunk::isVisible() {
    // Visible / intention to be visible
    return this->state.load() != CHUNK_INTERNAL_UNLOADED || should_build_mesh;
}

void Chunk::set(ivec3 voxel, char value) {
    this->voxels[idx(voxel)] = value;
    this->should_build_mesh = true;
}

void Chunk::shouldRefreshMesh() {
    this->should_build_mesh = true;
}