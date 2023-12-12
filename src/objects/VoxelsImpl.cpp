#include "VoxelsImpl.hpp"

ChunkImpl *VoxelsImpl::impl(std::shared_ptr<tinyrender::Chunk> &chunk)
{
    return chunk->chunk.get();
}

void VoxelsImpl::onInit(Context *c, Scene *s)
{
    ObjectImpl::onInit(c, s);

    globalModelMatrix = std::make_shared<ModelMatrixUniform>(c);
    voxelColors = std::make_shared<Texture2D::CharColorTexture>(c);

    std::array<uint8_t, 4*256> colors;
    colors.fill(255);
    voxelColors->update(colors);
}

void 
VoxelsImpl::onUpdate(State &state, float dt) {
    (void) dt;

    // Keep mesh queue builder moving
    meshQueueBuilder->update();

    if(state.frame % TICKS_PER_UPDATE != 0)
        return;

    // Update each chunk
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

std::shared_ptr<Texture2D::CharColorTexture> VoxelsImpl::colorTexture() 
{
    return voxelColors;
}

std::shared_ptr<tinyrender::Chunk> VoxelsImpl::getChunk(ivec2 chunkCoordinate)
{
    auto key = serializeChunkCoord(chunkCoordinate);
    if(map.count(key) > 0)
        return map.at(key);
    
    auto chunk = std::make_shared<tinyrender::Chunk>(*this, context, scene, chunkCoordinate);
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

void 
VoxelsImpl::setColors(char colorScheme) {
    std::array<uint8_t, 4*256> colors;
    switch(colorScheme) {
        case tinyrender::VOXEL_COL_GRAYSCALE: {
            for(int i = 0; i < 256; i++) {
                auto it = colors.begin() + 4*i;
                *it = i; *(it+1) = i; *(it+2) = i; *(it+3) = 255;
            }
            break;
        }
        case tinyrender::VOXEL_COL_RAINBOW_1: {
            for(int i = 0; i < 256; i++) {
                auto it = colors.begin() + 4*i;
                double g = (1.0-i/255.0)/0.25;
                int x = static_cast<int>(std::floor(g));
                int y = static_cast<int>(std::floor(255 * (g - x)));
                if(x == 0) {
                    *it = 255; *(it+1) = y; *(it+2) = 0; *(it+3) = 255;
                } else if(x == 1) {
                    *it = 255-y; *(it+1) = 255; *(it+2) = 0; *(it+3) = 255;
                } else if(x == 2) {
                    *it = 0; *(it+1) = 255; *(it+2) = y; *(it+3) = 255;
                } else if(x == 3) {
                    *it = 0; *(it+1) = 255-y; *(it+2) = 255; *(it+3) = 255;
                } else {
                    *it = 0; *(it+1) = 0; *(it+2) = 255; *(it+3) = 255;
                }
            }
            break;
        }
        case tinyrender::VOXEL_COL_RAINBOW_2: {
            for(int i = 0; i < 256; i++) {
                float t = i/255.0f * 31.4f;
                auto it = colors.begin() + 4*i;
                *it = 255 * (sin(t) + 1.0f)/2.0f;
                *(it+1) = 255 * (cos(t) + 1.0f)/2.0f;
                *(it+2) = 255 * (sin(t) + 1.0f)/2.0f;
                *(it+3) = 255;
            }
            break;
        }
        default:
            throw std::runtime_error("Invalid color scheme");
    }
    voxelColors->update(colors);
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

void
tinyrender::Voxels::setColors(char colorScheme) {
    static_cast<VoxelsImpl*>(obj.get())->setColors(colorScheme);
}
