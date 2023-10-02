#pragma once

#include <string>
#include <iostream>
#include "tiny_obj_loader.h"

#include "../../webgpu/primitives/buffers/AttributeStructs.hpp"
#include "Polygons.hpp"

void loadFromObjFile(std::string path, Polygons &p);
void loadFromObj(std::string defn, Polygons &p);