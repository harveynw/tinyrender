#include "IndexedAttribute.hpp"


engine::IndexedAttribute::IndexedAttribute(Context *context, IndexedTrianglePipelineData attrs) {
    // We only need vertexData from attrs
    std::vector<float> &v = attrs.vertexData;

    this->nDrawCalls = (int) v.size();

    // Buffer base class is initialised
    this->context = context;
    this->type = engine::VERTEX;
    this->size = v.size() * sizeof(float);
    this->mapped = false;
    this->initialise(v.data());
}