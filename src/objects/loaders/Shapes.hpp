#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Polygons.hpp"
#include "../../webgpu/primitives/buffers/AttributeStructs.hpp"

void loadTriangle(Polygons &p, glm::vec3 a, glm::vec3 b, glm::vec3 c);
void loadQuad(Polygons &p, glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d);
void loadPyramid(Polygons &p, glm::vec3 centre, float height, float width);
