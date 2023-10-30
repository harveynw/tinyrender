#pragma once

#include <stack>

#include "ChunkGeometry.hpp"
#include "VoxelData.hpp"
#include "../../webgpu/Scene.hpp"
#include "../ObjectResources.hpp"
#include "../loaders/Polygons.hpp"
#include "../loaders/Shapes.hpp"
#include "../../webgpu/primitives/buffers/AttributeBuffer.hpp"
#include "../../webgpu/Context.hpp"

namespace engine {
    class Voxels;
}

class Chunk;

struct ChunkNeighbours {
    // Along x axis
    std::weak_ptr<Chunk> west;
    std::weak_ptr<Chunk> east;

    // Along y axis
    std::weak_ptr<Chunk> north;
    std::weak_ptr<Chunk> south;
};

class Chunk {
    protected:
        Context *c;
        Scene *s;

        glm::ivec2 cornerCoordinate;
        std::array<char, N_VOXELS> voxels;

        std::shared_ptr<ObjectResources> resources;

        void syncBuffer();

        bool neighbourBlocked(glm::ivec3 outside);

        ChunkNeighbours neighbours{};

        friend class engine::Voxels;
    public:
        Chunk(Context *c, Scene *s, glm::ivec2 chunkCoordinate);

        std::shared_ptr<engine::AttributeBuffer> buffer = nullptr;
};
