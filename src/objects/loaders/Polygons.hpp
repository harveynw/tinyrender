#pragma once

#include <vector>

struct Polygons {
    std::vector<float> data; // Raw positions, normals, uv coords packed into vector of floats
    size_t vertices; // Number of vertices
};
