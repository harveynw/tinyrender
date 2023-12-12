#pragma once

#include "Object.hpp"
#include "voxel/Chunk.hpp"

class VoxelsImpl;

namespace tinyrender {

    class Voxels : public Object {
    protected:
        friend VoxelsImpl;
    public:
        Voxels();
        ~Voxels();

        std::shared_ptr<Chunk> getChunk(ivec2 chunkCoordinate);
        bool chunkTracked(ivec2 coord); 
        bool chunkDisplayed(ivec2 coord); 
        std::vector<std::shared_ptr<Chunk>> visibleChunks();
    };

}