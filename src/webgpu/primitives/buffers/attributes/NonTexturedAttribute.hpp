//
// Created by Harvey Williams on 17/08/2023.
//
#pragma once

#include "glm/glm.hpp"

#include "AttributeBuffer.hpp"


struct TriangleVertexAttributes {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
};


namespace engine {

    class NonTexturedAttribute : public engine::AttributeBuffer {
    public:
        explicit NonTexturedAttribute(Context *context, std::vector<TriangleVertexAttributes> attrs);
    };

}
