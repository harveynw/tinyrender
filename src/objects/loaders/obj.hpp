#pragma once

#include <string>
#include <iostream>
#include "tiny_obj_loader.h"

#include "../../webgpu/primitives/buffers/AttributeStructs.hpp"

struct Obj {
    std::vector<float> data; // Raw positions, normals, uv coords packed into vector of floats
    size_t vertices; // Number of vertices
};

void loadFromObjFile(std::string path, Obj &obj);
void loadFromObj(std::string defn, Obj &obj);