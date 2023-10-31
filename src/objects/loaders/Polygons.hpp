#pragma once

#include <vector>

struct Polygons {
    std::vector<float> data; // Raw positions, normals, uv coords packed into vector of floats
    size_t vertices; // Number of vertices

    Polygons &operator+=(const Polygons &rhs) {
        this->data.insert(this->data.end(), rhs.data.begin(), rhs.data.end());
        this->vertices += rhs.vertices;
        return *this;
    }

    Polygons() {
        data = {};
        vertices = 0;
    }
};

struct InstancedPolygons {
    std::vector<float> data; // Raw positions, normals, uv coords packed into vector of floats
    std::vector<uint16_t> indices; // Indices of data
};
