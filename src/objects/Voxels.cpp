#include "Voxels.hpp"

std::string engine::Voxels::serializeChunkCoord(glm::ivec2 coord)
{
    std::ostringstream ss;
    ss << coord.x << "," << coord.y;
    return ss.str();
}

void 
engine::Voxels::loadChunk(glm::ivec2 coord) {
    auto key = serializeChunkCoord(coord);
    if(loadedChunks.count(key) > 0)
        throw std::runtime_error("Chunk " + key + " already loaded");
    
    loadedChunks[key] = std::make_shared<Chunk>(this->context, this->scene, coord);
    linkNeighbours(coord);
}

void 
engine::Voxels::unloadChunk(glm::ivec2 coord) {
    auto key = serializeChunkCoord(coord);
    if(loadedChunks.count(key) < 1)
        throw std::runtime_error("Chunk " + key + " was not loaded");
    
    loadedChunks.erase(key);
    syncNeighbours(coord); // Rebuild the mesh of the neighbouring chunks
}

void 
engine::Voxels::linkNeighbours(glm::ivec2 coord) {
    auto chunk = loadedChunks[serializeChunkCoord(coord)];

    auto west = serializeChunkCoord(coord - glm::ivec2(1, 0));
    if(loadedChunks.count(west) > 0) {
        chunk->neighbours.west = loadedChunks[west];
        loadedChunks[west]->neighbours.east = chunk;
    }

    auto east = serializeChunkCoord(coord + glm::ivec2(1, 0));
    if(loadedChunks.count(east) > 0) {
        chunk->neighbours.east = loadedChunks[east];
        loadedChunks[east]->neighbours.west = chunk;
    }

    auto north = serializeChunkCoord(coord + glm::ivec2(0, 1));
    if(loadedChunks.count(north) > 0) {
        chunk->neighbours.north = loadedChunks[north];
        loadedChunks[north]->neighbours.south = chunk;
    }

    auto south = serializeChunkCoord(coord - glm::ivec2(0, 1));
    if(loadedChunks.count(south) > 0) {
        chunk->neighbours.south = loadedChunks[south];
        loadedChunks[south]->neighbours.north = chunk;
    }
}

void 
engine::Voxels::syncNeighbours(glm::ivec2 coord) {
    auto chunk = loadedChunks[serializeChunkCoord(coord)];
    if(auto n = chunk->neighbours.north.lock())
        n->syncBuffer();
    if(auto n = chunk->neighbours.south.lock())
        n->syncBuffer();
    if(auto n = chunk->neighbours.east.lock())
        n->syncBuffer();
    if(auto n = chunk->neighbours.west.lock())
        n->syncBuffer();
}

engine::Voxels::Voxels()
{
}

void engine::Voxels::onInit(Context *c, Scene *s) {
    Object::onInit(c, s);

    int h_size = 5;
    for(int x = -h_size; x <= h_size; x++) {
        for(int y = -h_size; y <= h_size; y++) {
            loadChunk(glm::ivec2(x, y));
        }
    }
}

void 
engine::Voxels::onUpdate(float dt) {
    (void) dt;

    for(auto &c: loadedChunks)
        c.second->onUpdate();
}

void 
engine::Voxels::onDraw(wgpu::RenderPassEncoder &renderPass, int vertexBufferSlot, int bindGroupSlot) {
    // Chunks are drawn individually so we override Object::onDraw
    for(auto &loaded : loadedChunks) {
        auto &c = loaded.second;
        if(c->state.load() != CHUNK_LOADED)
            continue;

        renderPass.setVertexBuffer(vertexBufferSlot, c->resources->attributeBuffer->getUnderlyingBuffer(), 0, c->resources->attributeBuffer->getSize());
        renderPass.setBindGroup(bindGroupSlot, c->resources->bindGroup, 0, nullptr);
        renderPass.draw(c->resources->attributeBuffer->getDrawCalls(), 1, 0, 0);
    }
}

void engine::Voxels::onRemove() {
    Object::onRemove();

    loadedChunks.clear();
}

engine::Voxels::~Voxels() {
    return;
}