#pragma once

#include "Object.hpp"

namespace tinyrender {
    class Voxels : public Object {
        std::shared_ptr<ChunkMap> map; 

        // Handles its own coloring 
        using Object::setColor;
        using Object::setTexture;
        using Object::modelMatrix;
    public:
        Voxels() = default;

        void onInit(Context *c, Scene *s) override;
        void onUpdate(State &state, float dt) override;
        void onDraw(wgpu::RenderPassEncoder &renderPass, int vertexBufferSlot, int bindGroupSlot) override;
        void onRemove() override;

        ~Voxels();
    };
}