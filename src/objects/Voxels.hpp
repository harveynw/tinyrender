#pragma once

#include "Object.hpp"
#include "loaders/Shapes.hpp"


#include <glm/glm.hpp>
#include <stack>

#define SIZE_X 16
#define SIZE_Y 16
#define SIZE_Z 256
#define N_VOXELS (SIZE_X * SIZE_Y * SIZE_Z)

namespace {
    class Chunk;
}


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