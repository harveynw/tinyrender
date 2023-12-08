#pragma once

#include "Object.hpp"
#include "voxel/Chunk.hpp"


namespace tinyrender {
    class Voxels : public Object {
        std::shared_ptr<Chunks> chunks; 

        // Handles its own coloring 
        using Object::setColor;
        using Object::setTexture;

        std::shared_ptr<tinyrender::ModelMatrixUniform> globalModelMatrix;
    public:
        Voxels() = default;

        void onInit(Context *c, Scene *s) override;
        void onUpdate(State &state, float dt) override;
        void onDraw(wgpu::RenderPassEncoder &renderPass, int vertexBufferSlot, int bindGroupSlot) override;
        void onRemove() override;

        ObjectPipeline currentTargetPipeline() override;

        std::shared_ptr<tinyrender::ModelMatrixUniform> modelMatrix() const override;

        // Pass through methods for chunks
        std::shared_ptr<Chunk> getChunk(ivec2 chunkCoordinate);
        bool chunkTracked(ivec2 coord); // Instance of chunk exists
        bool chunkDisplayed(ivec2 coord); // Chunk is being rendered
        std::vector<std::shared_ptr<Chunk>> visibleChunks();

        ~Voxels();
    };
}