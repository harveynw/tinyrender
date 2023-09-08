#include "IndexBuffer.hpp"

engine::IndexBuffer::IndexBuffer(Engine *engine, std::vector<uint16_t> indexData) {
    this->nDrawCalls = (int) indexData.size();

    // Buffer base class is initialised
    this->engine = engine;
    this->type = engine::INDEX;
    this->size = indexData.size() * sizeof(uint16_t);
    this->mapped = false;
    this->initialise(indexData.data());
}