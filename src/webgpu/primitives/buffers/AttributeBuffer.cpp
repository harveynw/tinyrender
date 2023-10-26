#include "AttributeBuffer.hpp"


engine::AttributeBuffer::AttributeBuffer(Context *context, std::vector<float> &data, int nDrawCalls) {
    this->nDrawCalls = nDrawCalls;
    this->cpuBuffer = std::move(data);
    this->initialSize = cpuBuffer.size() * sizeof(float);

    /*
    printf("Loading Attributes:\n");
    for(auto f : cpuBuffer)
        printf("%f, ", f);
    printf("\n");
    */

    // Buffer data
    this->context = context;
    this->type = VERTEX;
    this->size = cpuBuffer.size() * sizeof(float);
    this->mapped = false;
    this->initialise(this->cpuBuffer.data());
}

void
engine::AttributeBuffer::update(std::vector<float> &newData, int newDrawCalls) {
    if(newData.size() > this->initialSize)
        throw std::runtime_error("Asked to fill buffer with more floats than initially allocated");

    this->nDrawCalls = newDrawCalls;
    context->queue.writeBuffer(underlying, 0, newData.data(), newData.size());
}
