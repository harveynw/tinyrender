//
// Created by Harvey Williams on 17/08/2023.
//
#pragma once

#include <glm/glm.hpp>

#include "AttributeBuffer.hpp"

struct IndexedTrianglePipelineData {
    std::vector<float> vertexData; // position R^3 + normal R^3 + color R^3
    std::vector<uint16_t> indexData; // i_0, i_1, ...
};


namespace engine {

    class IndexedAttributeBuffer : public engine::AttributeBuffer {
    public:
        explicit IndexedAttributeBuffer(Engine *engine, IndexedTrianglePipelineData attrs);
    };

}
