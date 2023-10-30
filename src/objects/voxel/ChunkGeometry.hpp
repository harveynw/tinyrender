#pragma once

#include <vector>
#include <stack>
#include <unordered_map>
#include <glm/glm.hpp>

// Chunk dimensions
#define SIZE_X 16
#define SIZE_Y 16
#define SIZE_Z 100
#define N_VOXELS (SIZE_X * SIZE_Y * SIZE_Z)

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
