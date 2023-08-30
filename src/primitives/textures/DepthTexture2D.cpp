//
// Created by Harvey Williams on 11/08/2023.
//

#include "DepthTexture2D.hpp"

engine::DepthTexture2D::DepthTexture2D(Engine *engine, uint32_t width, uint32_t height) {
    this->engine = engine;

    textureDesc.dimension = TextureDimension::_2D;
    textureDesc.format = textureFormat;
    textureDesc.mipLevelCount = 1;
    textureDesc.sampleCount = 1;
    textureDesc.size = {width, height, 1};
    textureDesc.usage = TextureUsage::RenderAttachment;
    textureDesc.viewFormatCount = 1;
    textureDesc.viewFormats = (WGPUTextureFormat*)&textureFormat;
    underlying = engine->wgpuGetDevice().createTexture(textureDesc);
    std::cout << "Depth texture: " << underlying << std::endl;

    // Create the view of the depth texture to be manipulated by the rasterizer
    TextureViewDescriptor depthTextureViewDesc;
    depthTextureViewDesc.aspect = TextureAspect::DepthOnly;
    depthTextureViewDesc.baseArrayLayer = 0;
    depthTextureViewDesc.arrayLayerCount = 1;
    depthTextureViewDesc.baseMipLevel = 0;
    depthTextureViewDesc.mipLevelCount = 1;
    depthTextureViewDesc.dimension = TextureViewDimension::_2D;
    depthTextureViewDesc.format = textureFormat;
    view = underlying.createView(depthTextureViewDesc);
    std::cout << "Depth texture view: " << view << std::endl;
}
