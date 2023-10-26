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
        Voxels();

        void onInit(Context *c, Scene *s) override;
        void onUpdate(float dt) override;
        void onRemove() override;

        ~Voxels();
    };
}