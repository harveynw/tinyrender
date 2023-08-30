#include "IndexedAttributeBuffer.hpp"


engine::IndexedAttributeBuffer::IndexedAttributeBuffer(Engine *engine, IndexedTrianglePipelineData attrs) {
    // We only need vertexData from attrs
    std::vector<float> &v = attrs.vertexData;

    this->nDrawCalls = (int) v.size();

    // Buffer base class is initialised
    this->engine = engine;
    this->type = engine::VERTEX;
    this->size = v.size() * sizeof(float);
    this->mapped = false;
    this->initialise(v.data());
}