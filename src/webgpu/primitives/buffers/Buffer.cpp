//
// Created by Harvey Williams on 09/08/2023.
//

#include "Buffer.hpp"


tr::Buffer::Buffer(Context *context, BufferType type, uint64_t size, bool mapped, void *data): context(context),
type(type), size(size), mapped(mapped) {
    initialise(data);
}

tr::Buffer::~Buffer() {
    if(underlying != nullptr) {
        underlying.destroy();
        underlying.release();
    }
}

void
tr::Buffer::initialise(void *data) {
    // Create the buffer
    wgpu::BufferDescriptor bufferDesc;
    bufferDesc.size = size;
    bufferDesc.usage = type;
    bufferDesc.mappedAtCreation = mapped;
    underlying = context->device.createBuffer(bufferDesc);

    // Write data via the queue
    context->queue.writeBuffer(underlying, 0, data, bufferDesc.size);
}
