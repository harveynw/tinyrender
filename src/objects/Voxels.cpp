#include "objects/Voxels.hpp"
#include "State.hpp"
#include "voxel/Chunk.hpp"
#include "voxel/Chunks.hpp"
#include "loaders/Shapes.hpp"
#include "ObjectResources.hpp"

void 
tinyrender::Voxels::onInit(Context *c, Scene *s) {
    Object::onInit(c, s);

    globalModelMatrix = std::make_shared<tinyrender::ModelMatrixUniform>(c);

    chunks = std::make_shared<Chunks>(c, s, globalModelMatrix);
}

void 
tinyrender::Voxels::onUpdate(State &state, float dt) {
    (void) dt;

    if(state.frame % TICKS_PER_UPDATE != 0)
        return;

    int x = (int) (state.cameraPosition.x / (float) SIZE_XY);
    int y = (int) (state.cameraPosition.y / (float) SIZE_XY);
    auto cameraChunk = glm::ivec2(x, y);

    for(auto &coord : visibleFrom(cameraChunk))
        chunks->getChunk(coord)->setVisibility(CHUNK_VISIBLE);

    for(auto &loaded: chunks->map) {
        auto chunk = loaded.second;
        if(!visible(cameraChunk, chunk->chunkCoordinate))
            chunk->setVisibility(CHUNK_HIDDEN);
        chunk->onUpdate(cameraChunk);
    }
}

void 
tinyrender::Voxels::onDraw(wgpu::RenderPassEncoder &renderPass, int vertexBufferSlot, int bindGroupSlot) {
    // Chunks are drawn individually so we override Object::onDraw
    for(auto &loaded : chunks->map)
        loaded.second->onDraw(renderPass, vertexBufferSlot, bindGroupSlot);
}

void tinyrender::Voxels::onRemove() {
    Object::onRemove();

    chunks = nullptr;
}

tinyrender::Voxels::~Voxels() {
    return;
}

ObjectPipeline tinyrender::Voxels::currentTargetPipeline() {
    return ObjectPipeline::Voxels;
}

std::shared_ptr<tinyrender::ModelMatrixUniform> tinyrender::Voxels::modelMatrix() const
{
    return globalModelMatrix;
}