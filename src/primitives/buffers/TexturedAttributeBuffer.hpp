//
// Created by Harvey Williams on 17/08/2023.
//
#pragma once

#include <glm/glm.hpp>

#include "AttributeBuffer.hpp"

struct UVTriangleVertexAttributes {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
    glm::vec2 uv;
};

namespace engine {

    class TexturedAttributeBuffer : public engine::AttributeBuffer {
    public:
        explicit TexturedAttributeBuffer(Engine *engine, std::vector<UVTriangleVertexAttributes> attrs);
    };

}
