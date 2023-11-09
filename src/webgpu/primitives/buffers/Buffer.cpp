//
// Created by Harvey Williams on 09/08/2023.
//

#include "Buffer.hpp"


tinyrender::Buffer::Buffer(Context *context, tinyrender::BufferType type, uint64_t size, bool mapped, void *data): context(context),
type(type), size(size), mapped(mapped) {
    initialise(data);
}

tinyrender::Buffer::~Buffer() {
    if(underlying != nullptr) {
        underlying.destroy();
        underlying.release();
    }
}

void
tinyrender::Buffer::initialise(void *data) {
    // Create the buffer
    wgpu::BufferDescriptor bufferDesc;
    bufferDesc.size = size;
    bufferDesc.usage = type;
    bufferDesc.mappedAtCreation = mapped;
    underlying = context->device.createBuffer(bufferDesc);

    // Write data via the queue
    context->queue.writeBuffer(underlying, 0, data, bufferDesc.size);
}

BindGroupEntry tinyrender::Buffer::generateUniformBindGroupEntry(int bindGroup) {
    // BindGroups are for uniforms
    assert(type == tinyrender::UNIFORM);

    BindGroupEntry entry;

    // Create a binding
    // The index of the binding (the entries in bindGroupDesc can be in any order)
    entry.binding = bindGroup;
    // The buffer it is actually bound to
    entry.buffer = underlying;
    // We can specify an offset within the buffer, so that a single buffer can hold
    // multiple uniform blocks.
    entry.offset = 0;
    // And we specify again the size of the buffer.
    entry.size = getSize();

    return entry;
}
