#include "AttributeBuffer.hpp"


engine::AttributeBuffer::AttributeBuffer(Context *context, std::vector<float> &data, int nDrawCalls) {
    this->nDrawCalls = nDrawCalls;
    this->initialSize = data.size();

    // Buffer data
    this->context = context;
    this->type = VERTEX;
    this->size = data.size();
    this->mapped = false;
    this->initialise(data.data());
}

void
engine::AttributeBuffer::update(std::vector<float> &newData, int newDrawCalls) {
    if(newData.size() > this->initialSize)
        throw std::runtime_error("Asked to fill buffer with more floats than initially allocated");

    this->nDrawCalls = newDrawCalls;
    context->queue.writeBuffer(underlying, 0, newData.data(), newData.size());
}
