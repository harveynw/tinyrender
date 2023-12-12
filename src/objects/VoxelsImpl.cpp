#include "VoxelsImpl.hpp"

ChunkImpl *VoxelsImpl::impl(std::shared_ptr<tinyrender::Chunk> &chunk)
{
    return chunk->chunk.get();
}

void VoxelsImpl::onInit(Context *c, Scene *s)
{
    ObjectImpl::onInit(c, s);

    globalModelMatrix = std::make_shared<ModelMatrixUniform>(c);
}

void 
VoxelsImpl::onUpdate(State &state, float dt) {
    (void) dt;
    if(state.frame % TICKS_PER_UPDATE != 0)
        return;

    for(auto &loaded : map) {
        auto chunk = loaded.second->chunk.get();
        chunk->onUpdate();
    }
}

void 
VoxelsImpl::onDraw(wgpu::RenderPassEncoder &renderPass, int vertexBufferSlot, int bindGroupSlot) {
    // Chunks are drawn individually so we override Object::onDraw
    for(auto &loaded : map) 
        loaded.second->chunk->onDraw(renderPass, vertexBufferSlot, bindGroupSlot);
}

void VoxelsImpl::onRemove() {
    ObjectImpl::onRemove();

    map.clear();
}

VoxelsImpl::~VoxelsImpl() {
    return;
}

ObjectPipeline VoxelsImpl::currentTargetPipeline() {
    return ObjectPipeline::Voxels;
}

std::shared_ptr<ModelMatrixUniform> VoxelsImpl::modelMatrix() const
{
    return globalModelMatrix;
}

std::shared_ptr<tinyrender::Chunk> VoxelsImpl::getChunk(ivec2 chunkCoordinate)
{
    auto key = serializeChunkCoord(chunkCoordinate);
    if(map.count(key) > 0)
        return map.at(key);
    
    auto chunk = std::make_shared<tinyrender::Chunk>(context, scene, *this, chunkCoordinate, globalModelMatrix);
    map[key] = chunk;
    return chunk;
}

bool VoxelsImpl::chunkTracked(ivec2 coord)
{
    return map.count(serializeChunkCoord(coord)) > 0;
}

bool VoxelsImpl::chunkDisplayed(ivec2 coord)
{
    return chunkTracked(coord) && getChunk(coord)->isVisible();
}

std::vector<std::shared_ptr<tinyrender::Chunk>> VoxelsImpl::visibleChunks()
{
    std::vector<std::shared_ptr<tinyrender::Chunk>> visible;
    for(auto &entry : map) {
        if(entry.second->isVisible())
            visible.push_back(entry.second);
    }
    return visible;
}

tinyrender::Voxels::Voxels() {
    obj = std::make_unique<VoxelsImpl>();
}

tinyrender::Voxels::~Voxels() = default;

std::shared_ptr<tinyrender::Chunk> 
tinyrender::Voxels::getChunk(ivec2 chunkCoordinate) {
    return static_cast<VoxelsImpl*>(obj.get())->getChunk(chunkCoordinate);
}

bool 
tinyrender::Voxels::chunkTracked(ivec2 coord) {
    return static_cast<VoxelsImpl*>(obj.get())->chunkTracked(coord);
}

bool 
tinyrender::Voxels::chunkDisplayed(ivec2 coord) {
    return static_cast<VoxelsImpl*>(obj.get())->chunkDisplayed(coord);
}

std::vector<std::shared_ptr<tinyrender::Chunk>> 
tinyrender::Voxels::visibleChunks() {
    return static_cast<VoxelsImpl*>(obj.get())->visibleChunks();
}
