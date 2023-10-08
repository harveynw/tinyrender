#pragma once

#include <glm/glm.hpp>


struct TriangleVertexAttributes {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
};

struct UVTriangleVertexAttributes {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
    glm::vec2 uv;
};
static_assert(sizeof(UVTriangleVertexAttributes) == sizeof(glm::vec3) * 3 + sizeof(glm::vec2));

