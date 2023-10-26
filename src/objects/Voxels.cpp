#include "Voxels.hpp"



engine::Voxels::Voxels()
{
}

void engine::Voxels::onInit(Context *c, Scene *s) {
    Object::onInit(c, s);

    // A single chunk for now
    chunk = std::make_shared<Chunk>(this->context);
    this->resources = std::make_shared<ObjectResources>(this->context, this->scene, this->chunk->buffer, ColoredTriangle);
}

void engine::Voxels::onUpdate(float dt) {
    (void) dt;
}

void engine::Voxels::onRemove() {
    Object::onRemove();

    this->resources.reset();
    this->chunk.reset();
}

engine::Voxels::~Voxels() {
    return;
}