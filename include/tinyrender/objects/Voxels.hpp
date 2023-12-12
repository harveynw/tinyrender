#pragma once

#include "Object.hpp"
#include "voxel/Chunk.hpp"

class VoxelsImpl;

namespace tinyrender {

    const char VOXEL_COL_GRAYSCALE = 0x00;
    const char VOXEL_COL_RAINBOW_1 = 0x01;
    const char VOXEL_COL_RAINBOW_2 = 0x02;

    class Voxels : public Object {
    protected:
        friend VoxelsImpl;
    public:
        Voxels();
        ~Voxels();

        // Get Chunk at chunkCoordinate, will initialise if not currently tracked
        std::shared_ptr<Chunk> getChunk(ivec2 chunkCoordinate);

        // Is chunk tracked
        bool chunkTracked(ivec2 coord); 

        // Is chunk visible
        bool chunkDisplayed(ivec2 coord); 

        // List visible chunks
        std::vector<std::shared_ptr<Chunk>> visibleChunks();

        // Set the char -> color mapping for the voxels
        void setColors(char colorScheme);
    };

}