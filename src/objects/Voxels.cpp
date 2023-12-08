#include "objects/Voxels.hpp"

#include "objects/voxel/Chunk.hpp"
#include "State.hpp"
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

    for(auto &loaded: chunks->map) {
        auto chunk = loaded.second;
        chunk->onUpdate();
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

std::shared_ptr<Chunk> tinyrender::Voxels::getChunk(ivec2 chunkCoordinate)
{
    return this->chunks->getChunk(chunkCoordinate);
}

bool tinyrender::Voxels::chunkTracked(ivec2 coord)
{
    return this->chunks->chunkTracked(coord);
}

bool tinyrender::Voxels::chunkDisplayed(ivec2 coord)
{
    return this->chunks->chunkDisplayed(coord);
}

std::vector<std::shared_ptr<Chunk>> tinyrender::Voxels::visibleChunks()
{
    return this->chunks->visibleChunks();
}
