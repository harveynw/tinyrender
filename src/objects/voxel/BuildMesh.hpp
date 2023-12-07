#pragma once

#include "Chunks.hpp"

/*
* This is the layout of each vertex attribute piped to the Shader on the GPU
*/
struct VoxelVertexAttribute {
    // Corner vertex position
    vec3 position; 
    // 0x?? Material
    // 0x?? Ambient Occlusion
    // 0x?? Normal Direction
    // 0x?? Unused
    float data; 
};
typedef vector<VoxelVertexAttribute> VoxelMesh;

struct NeighbourData {
    // Capture at specific time of voxels neighbouring a chunk
    array<char, SIZE_XY * SIZE_Z> north;
    array<char, SIZE_XY * SIZE_Z> south;
    array<char, SIZE_XY * SIZE_Z> east;
    array<char, SIZE_XY * SIZE_Z> west;

    array<char, SIZE_Z> cornerSW;
    array<char, SIZE_Z> cornerNW;
    array<char, SIZE_Z> cornerNE;
    array<char, SIZE_Z> cornerSE;
};

/*
* Helper methods, NOT thread safe
*/
NeighbourData extractBoundaries(Chunks &chunks, ivec2 chunkCoordinate);

/*
* Thread safe mesh building functions
*/
std::shared_ptr<VoxelMesh> buildMeshDFS(array<char, N_VOXELS> chunk, NeighbourData neighbourData);
std::shared_ptr<VoxelMesh> buildMeshGridSearch(array<char, N_VOXELS> chunk, NeighbourData neighbourData);
