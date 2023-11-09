#pragma once

#include "ChunkGeometry.hpp"
#include "../loaders/Polygons.hpp"
#include "../loaders/Shapes.hpp"
#include "../../webgpu/primitives/buffers/AttributeBuffer.hpp"

// Forward dec.
struct AdjacentMesh;
class Chunk;

class Mesh {
protected:
    // CPU Polygon Mesh
    std::unique_ptr<Polygons> mesh;

    // Occlusion at boundaries, stored as 4 2D slices 
    std::array<VoxelSlice, 4> occlusionData;

    bool boundaryOcclusion(Face &face);
public:
    // CPU Voxel Data
    VoxelChunk voxels;

    // Not thread safe
    void updateOcculusionData(AdjacentMesh &adj);

    // Thread safe
    void generateMesh();

    // Not thread safe
    std::shared_ptr<tinyrender::AttributeBuffer> gpu(Context *context);
};

struct AdjacentMesh {
    std::shared_ptr<Chunk> chunk;
    std::array<std::weak_ptr<Mesh>, 4> neighbours;

    AdjacentMesh(std::shared_ptr<Chunk> chunk): chunk(chunk) {};
    AdjacentMesh(): chunk(nullptr){};
};


