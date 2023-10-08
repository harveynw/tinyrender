#pragma once

#include "Texture2D.hpp"

namespace engine::Texture2D {

    class WavesDataTexture : public Texture {
    protected:
        wgpu::ImageCopyTexture destination;
        wgpu::TextureDataLayout source;
    public:
        WavesDataTexture(Context *context, int resWidth, int resLength) : Texture(context,
        new SolidColor(resWidth, resLength, glm::vec3(0.0, 0.0, 0.0)),
        new RGBA(),
        new RGBAView(),
        new DefaultSampler(true, false, false)) {
            // Setup destination and source, so we can use them to update the texture
            destination.texture = underlying;
            destination.origin = { 0, 0, 0 }; // equivalent of the offset argument of Queue::writeBuffer
            destination.aspect = wgpu::TextureAspect::All; // only relevant for depth/Stencil textures
            destination.mipLevel = 0;

            // Setup how data is laid out in C++
            source.offset = 0;
            source.bytesPerRow = 4 * width;
            source.rowsPerImage = height;
        }

        void update(std::vector<uint8_t> &displacement) {
            this->context->queue.writeTexture(destination, displacement.data(), displacement.size(),
                                        source,{width, height, 1});
        }
    };

}