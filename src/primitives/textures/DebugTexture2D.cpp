//
// Created by Harvey Williams on 12/08/2023.
//

#include "DebugTexture2D.hpp"

void
engine::DebugTexture2D::generateData() {
    data = std::vector<uint8_t>(4 * textureDesc.size.width * textureDesc.size.height);
    for (uint32_t i = 0; i < textureDesc.size.width; ++i) {
        for (uint32_t j = 0; j < textureDesc.size.height; ++j) {
            uint8_t *p = &data[4 * (j * textureDesc.size.width + i)];

            //p[0] = 255; p[1] = 255; p[2] = 255; p[3] = 255; // All white

            p[0] = (i / 16) % 2 == (j / 16) % 2 ? 255 : 0; // r
            p[1] = ((i - j) / 16) % 2 == 0 ? 255 : 0; // g
            p[2] = ((i + j) / 16) % 2 == 0 ? 255 : 0; // b
            p[3] = 255; //

            /* Smooth
            p[0] = (uint8_t)i; // r
            p[1] = (uint8_t)j; // g
            p[2] = 128; // b
            p[3] = 255; // a
             */
        }
    }
}

engine::DebugTexture2D::DebugTexture2D(Engine *engine, uint32_t width, uint32_t height) {
    this->engine = engine;

    // Setup texture descriptor
    textureDesc.dimension = TextureDimension::_2D;
    textureDesc.size = { width, height, 1 };
    textureDesc.mipLevelCount = 1;
    textureDesc.sampleCount = 1;
    textureDesc.format = TextureFormat::RGBA8Unorm;
    textureDesc.usage = TextureUsage::TextureBinding | TextureUsage::CopyDst;
    textureDesc.viewFormatCount = 0;
    textureDesc.viewFormats = nullptr;
    underlying = engine->wgpuGetDevice().createTexture(textureDesc);
    std::cout << "Texture: " << underlying << std::endl;

    // Setup copy from data to texture
    destination.texture = underlying;
    destination.mipLevel = 0;
    destination.origin = { 0, 0, 0 }; // equivalent of the offset argument of Queue::writeBuffer
    destination.aspect = TextureAspect::All; // only relevant for depth/Stencil textures

    // Setup how data is laid out in C++
    source.offset = 0;
    source.bytesPerRow = 4 * textureDesc.size.width;
    source.rowsPerImage = textureDesc.size.height;

    // Generate data and initialise
    generateData();
    size = data.size();
    engine->wgpuGetQueue().writeTexture(destination, data.data(), data.size(), source, textureDesc.size);

    // Create a view
    TextureViewDescriptor textureViewDesc;
    textureViewDesc.aspect = TextureAspect::All;
    textureViewDesc.baseArrayLayer = 0;
    textureViewDesc.arrayLayerCount = 1;
    textureViewDesc.baseMipLevel = 0;
    textureViewDesc.mipLevelCount = 1;
    textureViewDesc.dimension = TextureViewDimension::_2D;
    textureViewDesc.format = textureDesc.format;
    view = underlying.createView(textureViewDesc);
    std::cout << "Texture view: " << view << std::endl;
}
