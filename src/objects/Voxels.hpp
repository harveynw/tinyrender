#pragma once


#include "Object.hpp"
#include "voxel/Chunk.hpp"
#include "loaders/Shapes.hpp"


namespace engine {
    class Voxels : public Object {
        std::shared_ptr<ChunkMap> map; 

        // Handles its own coloring 
        using Object::setColor;
        using Object::setTexture;
        using Object::modelMatrix;
    public:
        Voxels();

        void onInit(Context *c, Scene *s) override;
        void onUpdate(State &state, float dt) override;
        void onDraw(wgpu::RenderPassEncoder &renderPass, int vertexBufferSlot, int bindGroupSlot) override;
        void onRemove() override;

        ~Voxels();
    };
}