//
// Created by Harvey Williams on 17/08/2023.
//

#include "TexturedAttributeBuffer.hpp"


engine::TexturedAttributeBuffer::TexturedAttributeBuffer(Engine *engine, std::vector<UVTriangleVertexAttributes> attrs) {
    this->nDrawCalls = (int) attrs.size();

    // Buffer base class is initialised
    this->engine = engine;
    this->type = engine::VERTEX;
    this->size = attrs.size() * sizeof(UVTriangleVertexAttributes);
    this->mapped = false;
    this->initialise(attrs.data());
}
