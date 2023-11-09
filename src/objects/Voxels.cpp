#include "Voxels.hpp"


tinyrender::Voxels::Voxels() {}

void 
tinyrender::Voxels::onInit(Context *c, Scene *s) {
    Object::onInit(c, s);
    map = std::make_shared<ChunkMap>(c, s);
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
        this->map->ensureLoaded(coord);
    for(auto &loaded : this->map->all())
        loaded->onUpdate(cameraChunk);

}

void 
tinyrender::Voxels::onDraw(wgpu::RenderPassEncoder &renderPass, int vertexBufferSlot, int bindGroupSlot) {
    // Chunks are drawn individually so we override Object::onDraw
    for(auto &loaded : this->map->all())
        loaded->onDraw(renderPass, vertexBufferSlot, bindGroupSlot);
}

void tinyrender::Voxels::onRemove() {
    Object::onRemove();

    map = nullptr;
}

tinyrender::Voxels::~Voxels() {
    return;
}