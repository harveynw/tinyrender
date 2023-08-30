//
// Created by Harvey Williams on 09/08/2023.
//

#include "Texture2D.hpp"

engine::Texture2D::~Texture2D() {
    if(view != nullptr) {
        view.release();
    }
    if(underlying != nullptr) {
        underlying.destroy();
        underlying.release();
    }
}

engine::Texture2D::Texture2D(Engine *engine, wgpu::TextureDescriptor textureDesc,
                             wgpu::ImageCopyTexture destination, wgpu::TextureDataLayout source, uint64_t size,
                             void *data):
                             engine(engine), size(size), textureDesc(textureDesc), destination(destination),
                             source(source) {
    // Init
    initialise(data);
}

void
engine::Texture2D::initialise(void *data) {
    // Create texture
    underlying = engine->wgpuGetDevice().createTexture(textureDesc);

    // Write data
    engine->wgpuGetQueue().writeTexture(destination, data, size, source, textureDesc.size);
}
