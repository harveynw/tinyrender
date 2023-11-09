#pragma once

#include <vector>
#include <stack>
#include <map>
#include <unordered_map>
#include <glm/glm.hpp>
#include <sstream>
#include <cstdlib>

class Chunk;

// Chunk engine config
const uint64_t TICKS_PER_UPDATE = 10; // How many ticks to check whether to load chunks
const int VISIBILITY_DISTANCE = 10; // How many chunks around the camera to keep loaded

// Chunk dimensions
#define SIZE_XY 16
#define SIZE_Z 100
#define N_VOXELS (SIZE_XY * SIZE_XY * SIZE_Z)

// For adjacent chunks 
typedef int DIRECTION;
const DIRECTION WEST = 0;
const DIRECTION EAST = 1;
const DIRECTION NORTH = 2;
const DIRECTION SOUTH = 3;
const std::vector<DIRECTION> directions = {NORTH, SOUTH, EAST, WEST};
DIRECTION opposite(DIRECTION d);
glm::ivec2 chunkDirection(DIRECTION d);

// 3D and 2D voxel data respectively
typedef std::array<char, SIZE_XY * SIZE_XY * SIZE_Z> VoxelChunk;
typedef std::array<char, SIZE_XY * SIZE_Z> VoxelSlice;

struct Face {
    glm::ivec3 from; // May be from outside chunk
    glm::ivec3 to; // Always inside chunk
};

typedef std::vector<glm::ivec3> Coordinates;
typedef std::vector<Face> Faces;

// Array utilty methods
int idx(glm::ivec3 c);
int hash(glm::ivec3 c);
glm::ivec3 coord(int i);
bool inBounds(glm::ivec3 &c);
Coordinates adjacentCoordinates(glm::ivec3 &c);
Faces visibleFrom(glm::ivec3 &c0, std::array<char, N_VOXELS> &voxels);
bool isTransparent(glm::ivec3 &c, std::array<char, N_VOXELS> &voxels);
bool onBoundary(glm::ivec3 &c);
#define MOD(a, b) (((a % b) + b) % b)
glm::ivec3 wrapCoordinate(glm::ivec3 &c);

// Chunk utility methods
std::string serializeChunkCoord(glm::ivec2 coord);
bool visible(glm::ivec2 chunkA, glm::ivec2 chunkB);
std::vector<glm::ivec2> visibleFrom(glm::ivec2 chunk);
