#pragma once

#include <stack>

#include "ChunkGeometry.hpp"
#include "VoxelData.hpp"
#include "../loaders/Polygons.hpp"
#include "../loaders/Shapes.hpp"
#include "../../webgpu/primitives/buffers/AttributeBuffer.hpp"
#include "../../webgpu/Context.hpp"


class Chunk {
    protected:
        Context *c;
        std::array<char, N_VOXELS> voxels;

        void syncBuffer();
    public:
        Chunk(Context *c);

        std::shared_ptr<engine::AttributeBuffer> buffer = nullptr;
};
