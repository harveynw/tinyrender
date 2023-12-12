#pragma once

#include <memory>

#include "ChunkGeometry.hpp"

class ChunkImpl;
class VoxelsImpl;
struct Context;
struct Scene;


namespace tinyrender {

    const char CHUNK_VISIBLE = 0x00;
    const char CHUNK_HIDDEN = 0x01;

    class Chunk {
    protected:
        std::unique_ptr<ChunkImpl> chunk;

        friend ChunkImpl;
        friend VoxelsImpl;
    public:
        Chunk(VoxelsImpl &v, Context *c, Scene *s, ivec2 chunkCoordinate);
        ~Chunk();

        std::array<char, N_VOXELS>& voxelData();
        void set(ivec3, char);
        ivec2& cornerCoordinate();
        ivec2& chunkCoordinate();
        void setVisibility(char);
        bool isVisible();
        void shouldRefreshMesh();
    };
}