//
// Created by Harvey Williams on 17/08/2023.
//

#include "TexturedAttribute.hpp"


engine::TexturedAttribute::TexturedAttribute(Context *context, std::vector<UVTriangleVertexAttributes> attrs) {
    this->nDrawCalls = (int) attrs.size();

    // Buffer base class is initialised
    this->context = context;
    this->type = engine::VERTEX;
    this->size = attrs.size() * sizeof(UVTriangleVertexAttributes);
    this->mapped = false;
    this->initialise(attrs.data());
}
