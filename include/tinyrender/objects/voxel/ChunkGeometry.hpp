#pragma once

#include <vector>
#include <map>
#include <glm/glm.hpp>

using glm::ivec2;
using glm::ivec3;
using std::string;
using std::vector;
using std::array;

// Chunk engine config
const uint64_t TICKS_PER_UPDATE = 10; // How many ticks to check whether to load chunks
const int VISIBILITY_DISTANCE = 1; // How many chunks around the camera to keep loaded

// Chunk dimensions
#define SIZE_XY 16
#define SIZE_Z 100
#define N_VOXELS (SIZE_XY * SIZE_XY * SIZE_Z)

// Array utilty methods
int idx(ivec3 c);
int hash(ivec3 c);
ivec3 coord(int i);
bool inBounds(ivec3 &c);
vector<ivec3> adjacentCoordinates(ivec3 &c);
bool isTransparent(ivec3 &c, array<char, N_VOXELS> &voxels);
bool onBoundary(ivec3 &c);
#define MOD(a, b) (((a % b) + b) % b)
ivec3 wrapCoordinate(ivec3 &c);

// Chunk utility methods
string serializeChunkCoord(ivec2 coord);
