#pragma once

#include "ChunkGeometry.hpp"
#include "FastNoiseLite.h"

/*
* VoxelData
* Set of routines for populating a chunk of voxels
*/

void sphere(std::array<char, N_VOXELS> &voxels, int radius);
void fill(std::array<char, N_VOXELS> &voxels);
void minecraft(std::array<char, N_VOXELS> &voxels, glm::ivec2 cornerXY);