#include "AttributeBuffer.hpp"


engine::AttributeBuffer::AttributeBuffer(Context *context, std::vector<float> &data, int nDrawCalls) {
    this->nDrawCalls = nDrawCalls;

    // Buffer data
    this->context = context;
    this->type = VERTEX;
    this->size = data.size();
    this->mapped = false;
    this->initialise(data.data());
}
