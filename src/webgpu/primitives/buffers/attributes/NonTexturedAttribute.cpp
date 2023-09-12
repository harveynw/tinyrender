
//
// Created by Harvey Williams on 18/08/2023.
//

#include "NonTexturedAttribute.hpp"


engine::NonTexturedAttribute::NonTexturedAttribute(Context *context, std::vector<TriangleVertexAttributes> attrs) {
    this->nDrawCalls = (int) attrs.size();

    // Buffer base class is initialised
    this->context = context;
    this->type = engine::VERTEX;
    this->size = attrs.size() * sizeof(TriangleVertexAttributes);
    this->mapped = false;
    this->initialise(attrs.data());
}
