#pragma once

#include "Texture2D.hpp"

/*
* CharColorTexture
* Provides a 256x1x3 size texture which gives a color for each possible value of a char.
*/
namespace Texture2D {

    class CharColorTexture : public Texture {
    protected:
        wgpu::ImageCopyTexture destination;
        wgpu::TextureDataLayout source;
    public:
        CharColorTexture(Context *context) : Texture(context,
        new SolidColor(256, 1, glm::vec3(0.0, 0.0, 0.0)),
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
            source.bytesPerRow = 4 * 256;
            source.rowsPerImage = 1;
        }

        void update(std::array<uint8_t, 4*256> &colors) {
            this->context->queue.writeTexture(destination, colors.data(), colors.size(),
                                        source, {256, 1, 1});
        }
    };

}