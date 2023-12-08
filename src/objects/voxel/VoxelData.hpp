#pragma once

#include "objects/voxel/ChunkGeometry.hpp"

/*
* VoxelData
* Set of routines for populating a chunk of voxels
*/

void sphere(std::array<char, N_VOXELS> &voxels, int radius);
void fill(std::array<char, N_VOXELS> &voxels);