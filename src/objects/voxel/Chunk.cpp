#include "Chunk.hpp"


void 
Chunk::onUpdate(glm::ivec2 cameraChunk) {
    bool shouldBeVisible = visible(cameraChunk, this->chunkCoordinate);

    // Enter state machine 
    switch(auto s = this->state.load()) {
        case CHUNK_SHOULD_GENERATE: {
            this->state.store(CHUNK_GENERATING_MESH);
            this->refresh = false;
            if(this->mesh == nullptr)
                this->mesh = std::make_unique<Mesh>();

            // First update neighbour data on main thread
            AdjacentMesh nbs = this->chunkMap->neighbours(this->chunkCoordinate);
            this->mesh->updateOcculusionData(nbs);

            // Can now generate mesh off main thread.
            loadingThread = std::thread([&]{
                this->mesh->generateMesh();
                this->state.store(CHUNK_MESH_READY);
            }); 
            loadingThread.detach();
            break;
        }
        case CHUNK_MESH_READY: {
            this->syncBuffer();
            this->state.store(CHUNK_LOADED);
            break;
        }
        case CHUNK_LOADED: {
            if(!shouldBeVisible) {
                // Clear
                this->refresh = false;
                this->resources = nullptr;

                this->state.store(CHUNK_NO_MESH);
            } else if(this->refresh) {
                this->state.store(CHUNK_SHOULD_GENERATE);
            }
            break;
        }
        case CHUNK_NO_MESH: {
            if(shouldBeVisible) {
                this->state.store(CHUNK_SHOULD_GENERATE);
            }
            break;
        }
        case CHUNK_GENERATING_MESH:
        default:
            break;
    }
}

void 
Chunk::shouldRefresh() {
    this->refresh = true;
}

std::shared_ptr<Mesh> 
Chunk::getMesh() {
    return mesh;
}

void 
Chunk::syncBuffer() {
    if(this->state.load() != CHUNK_MESH_READY || this->mesh == nullptr)
        throw std::runtime_error("Not in state to update GPU buffer");
    
    buffer = mesh->gpu(this->c);
    resources = std::make_shared<ObjectResources>(this->c, this->s, this->buffer, ColoredTriangle);
    resources->modelMatrix->setTranslation(glm::vec3(this->cornerCoordinate, 0.0));
}


Chunk::Chunk(Context *context, Scene *scene, ChunkMap *chunkMap, glm::ivec2 chunkCoordinate): c(context), s(scene), chunkMap(chunkMap) {
    this->chunkCoordinate = chunkCoordinate;
    this->cornerCoordinate = glm::ivec2(SIZE_XY, SIZE_XY) * chunkCoordinate;

    this->mesh = std::make_unique<Mesh>();
    minecraft(this->mesh->voxels, this->cornerCoordinate);
}

void
Chunk::onDraw(wgpu::RenderPassEncoder &renderPass, int vertexBufferSlot, int bindGroupSlot) {
    if(this->resources == nullptr || this->resources->attributeBuffer->getDrawCalls() == 0)
        return;

    renderPass.setVertexBuffer(vertexBufferSlot, this->resources->attributeBuffer->getUnderlyingBuffer(), 0, this->resources->attributeBuffer->getSize());
    renderPass.setBindGroup(bindGroupSlot, this->resources->bindGroup, 0, nullptr);
    renderPass.draw(this->resources->attributeBuffer->getDrawCalls(), 1, 0, 0);
}

void 
ChunkMap::updateNeighbours(glm::ivec2 chunkCoordinate) {
    // Update 'map' with up to date pointers, propagate a shouldRefresh()
    std::string key = serializeChunkCoord(chunkCoordinate);
    std::shared_ptr<Chunk> chunk = nullptr;
    if(chunks.count(key) > 0)
        chunk = chunks[key];

    AdjacentMesh *adj = &map[key];

    // Update and refresh the neighbours
    for(DIRECTION d : directions) {
        auto c = serializeChunkCoord(chunkCoordinate + chunkDirection(d));
        if(map.count(c) > 0) {
            adj->neighbours[d] = map[c].chunk->getMesh();
            map[c].neighbours[opposite(d)] = chunk ? chunk->getMesh() : nullptr;
        }
    }
}

ChunkMap::ChunkMap(Context *context, Scene *scene) : context(context), scene(scene) {}

std::shared_ptr<Chunk> 
ChunkMap::ensureLoaded(glm::ivec2 chunkCoordinate) {
    auto key = serializeChunkCoord(chunkCoordinate);
    if(chunks.count(key) > 0)
        return chunks[key];

    chunks.insert({key, std::make_shared<Chunk>(this->context, this->scene, this, chunkCoordinate)});
    chunksSequential.push_back(chunks[key]);

    std::shared_ptr<Chunk> chunk = chunks[key];

    map.insert({key, AdjacentMesh(chunk)});
    updateNeighbours(chunkCoordinate);
    refreshNeighbours(chunkCoordinate);

    return chunk;
}

std::shared_ptr<Chunk> 
ChunkMap::chunkAt(glm::ivec2 chunkCoordinate) {
    auto key = serializeChunkCoord(chunkCoordinate);
    if(chunks.count(key) == 0)
        throw std::runtime_error("Chunk not found");

    return chunks[key];
}

void 
ChunkMap::refreshNeighbours(glm::ivec2 chunkCoordinate) {
    // Call shouldRefresh() on neighbouring chunks
    std::string key = serializeChunkCoord(chunkCoordinate);
    
    auto west = serializeChunkCoord(chunkCoordinate - glm::ivec2(1, 0));
    if(map.count(west) > 0)
        map[west].chunk->shouldRefresh(); 

    auto east = serializeChunkCoord(chunkCoordinate + glm::ivec2(1, 0));
    if(map.count(east) > 0)
        map[east].chunk->shouldRefresh();

    auto north = serializeChunkCoord(chunkCoordinate + glm::ivec2(0, 1));
    if(map.count(north) > 0) 
        map[north].chunk->shouldRefresh();

    auto south = serializeChunkCoord(chunkCoordinate - glm::ivec2(0, 1));
    if(map.count(south) > 0)
        map[south].chunk->shouldRefresh();
}

AdjacentMesh 
ChunkMap::neighbours(glm::ivec2 chunkCoordinate) {
    return this->map[serializeChunkCoord(chunkCoordinate)];
}

std::vector<std::shared_ptr<Chunk>> 
ChunkMap::all() {
    return this->chunksSequential;
}

