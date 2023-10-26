#pragma once

#include "Object.hpp"
#include "voxel/Chunk.hpp"
#include "loaders/Shapes.hpp"


namespace engine {
    class Voxels : public Object {
        std::shared_ptr<Chunk> chunk = nullptr;

        // Handles its own coloring 
        using Object::setColor;
        using Object::setTexture;
    public:
        Voxels(Context *c, Scene *s);
        void update(float dt) override; 
        ~Voxels();
    };
}