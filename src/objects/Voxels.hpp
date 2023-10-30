#pragma once

#include <map>
#include <sstream>

#include "Object.hpp"
#include "voxel/Chunk.hpp"
#include "loaders/Shapes.hpp"


namespace engine {
    class Voxels : public Object {
        std::map<std::string, std::shared_ptr<Chunk>> loadedChunks;

        // Handles its own coloring 
        using Object::setColor;
        using Object::setTexture;
        using Object::modelMatrix;

        std::string serializeChunkCoord(glm::ivec2 coord);
        void loadChunk(glm::ivec2 coord);
        void unloadChunk(glm::ivec2 coord);
        void linkNeighbours(glm::ivec2 coord);
        void syncNeighbours(glm::ivec2 coord);
    public:
        Voxels();

        void onInit(Context *c, Scene *s) override;
        void onUpdate(float dt) override;
        void onDraw(wgpu::RenderPassEncoder &renderPass, int vertexBufferSlot, int bindGroupSlot) override;
        void onRemove() override;

        ~Voxels();
    };
}