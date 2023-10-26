#include "Voxels.hpp"



engine::Voxels::Voxels(Context *c, Scene *s)
{
    this->context = c;
    this->scene = s;

    // Temporarily one chunk
    chunk = std::make_shared<Chunk>(this->context);
    this->resources = std::make_shared<ObjectResources>(this->context, this->scene, this->chunk->buffer, ColoredTriangle);
}

void engine::Voxels::update(float dt)
{
    (void) dt;
}

engine::Voxels::~Voxels()
{
}