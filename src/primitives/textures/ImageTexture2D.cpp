//
// Created by Harvey Williams on 12/08/2023.
//

#include "ImageTexture2D.hpp"

engine::ImageTexture2D::ImageTexture2D(Engine *engine, const fs::path& path) {
    this->engine = engine;

    int width, height, channels;
    unsigned char *pixelData = stbi_load(path.string().c_str(), &width, &height, &channels,
                                         4 /* force 4 channels */);
    if (nullptr == pixelData)
        throw std::runtime_error("Failed to extract pixel data");

    // Setup texture descriptor
    textureDesc.dimension = TextureDimension::_2D;
    textureDesc.size = { (unsigned int) width, (unsigned int) height, 1 };
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
    engine->wgpuGetQueue().writeTexture(destination, pixelData,
                                        4 * textureDesc.size.width * textureDesc.size.height,
                                        source, textureDesc.size);

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

    stbi_image_free(pixelData);
}