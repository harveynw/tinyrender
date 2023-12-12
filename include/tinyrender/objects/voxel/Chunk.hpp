#pragma once

#include <memory>

#include "ChunkGeometry.hpp"

class ChunkImpl;
class VoxelsImpl;
class ModelMatrixUniform;
struct Context;
struct Scene;

// Voxel API todo
// 
// 1d color texture 256 wide in shaders, configurable colors

namespace tinyrender {

    const char CHUNK_VISIBLE = 0x00;
    const char CHUNK_HIDDEN = 0x01;

    class Chunk {
    protected:
        std::unique_ptr<ChunkImpl> chunk;

        friend ChunkImpl;
        friend VoxelsImpl;
    public:
        Chunk(Context *c, Scene *s, VoxelsImpl &v, ivec2 chunkCoordinate, std::shared_ptr<ModelMatrixUniform> globalModelMatrix);
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