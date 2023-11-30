#pragma once

#include "Chunks.hpp"

/*
* This is the layout of each vertex attribute piped to the Shader on the GPU
*/
struct VoxelVertexAttribute {
    // Corner position
    vec3 position; 
    // 0x?? Material
    // 0x?? Ambient Occlusion
    // 0x?? Normal Direction
    // 0x?? Unused
    float data; 
};
typedef vector<VoxelVertexAttribute> VoxelMesh;

struct NeighbourData {
    // 2D slices of neighbouring voxels that determine whether there is occlusion or not
    array<bool, SIZE_XY * SIZE_Z> north;
    array<bool, SIZE_XY * SIZE_Z> south;
    array<bool, SIZE_XY * SIZE_Z> east;
    array<bool, SIZE_XY * SIZE_Z> west;

    void print() {
        int n = 0;
        int s = 0;
        int e = 0;
        int w = 0;
        float den = SIZE_XY * SIZE_Z;
        for(int i = 0; i < SIZE_XY * SIZE_Z; i++) {
            if(north[i])
                n++;
            if(south[i])
                s++;
            if(east[i])
                e++;
            if(west[i])
                w++;
        }
        printf("Neighbour data: %f, %f, %f, %f\n", (float) n/den, (float) s/den, (float) e/den, (float) w/den);
    };
};

/*
* Helper methods, NOT thread safe
*/
NeighbourData extractBoundaries(Chunks &chunks, ivec2 chunkCoordinate);


/*
* Thread safe mesh building functions
*/
std::shared_ptr<VoxelMesh> buildMeshCullBoundaries(array<char, N_VOXELS> chunk, NeighbourData neighbourData);
std::shared_ptr<VoxelMesh> buildMeshNaive(array<char, N_VOXELS> chunk);
std::shared_ptr<VoxelMesh> buildMeshGridSearch(array<char, N_VOXELS> chunk);
