
//
// Created by Harvey Williams on 18/08/2023.
//

#include "NonTexturedAttributeBuffer.hpp"


engine::NonTexturedAttributeBuffer::NonTexturedAttributeBuffer(Engine *engine, std::vector<TriangleVertexAttributes> attrs) {
    this->nDrawCalls = (int) attrs.size();

    // Buffer base class is initialised
    this->engine = engine;
    this->type = engine::VERTEX;
    this->size = attrs.size() * sizeof(TriangleVertexAttributes);
    this->mapped = false;
    this->initialise(attrs.data());
}
