#pragma once

#include <utility>
#include <webgpu/webgpu.hpp>
#include <filesystem>
#include "stb_image.h"

#include "../../Context.hpp"


namespace engine::Texture2D {

    class DataDelegate {
    public:
        virtual ~DataDelegate() = default;

        // Initialise populates width and height so the texture knows what size it is
        virtual void initialise(unsigned int &width, unsigned int &height, unsigned int &mips) = 0;
        // Write, to be called after initialise, is given the underlying texture to target and populate in GPU memory
        virtual void write(Context *context, wgpu::Texture underlying) = 0;
    };

    class NoData : public DataDelegate {
    public:
        ~NoData() override = default;

        void initialise(unsigned int &width, unsigned int &height, unsigned int &mips) override {
            (void) width;
            (void) height;
            (void) mips;
            // no-op
        }
        void write(Context *context, wgpu::Texture underlying) override {
            (void) context;
            (void) underlying;
            // no-op
        }
    };

    class DebugData : public DataDelegate {
    protected:
        unsigned int _width;
        unsigned int _height;

        static constexpr unsigned int n_mips = 8;
        std::array<std::vector<uint8_t>, n_mips> data;
    public:
        DebugData(unsigned int width, unsigned int height): _width(width), _height(height) {};
        ~DebugData() override = default;

        void initialise(unsigned int &width, unsigned int &height, unsigned int &mips) override {
            width = this->_width;
            height = this->_height;
            mips = n_mips;

            // Generate data and initialise
            uint32_t currentWidth = _width;
            uint32_t currentHeight = _height;
            for (uint32_t level = 0; level < n_mips; ++level) {
                // Allocate for current level
                data[level] = std::vector<uint8_t>(4 * currentWidth * currentHeight);

                // Set data
                auto &pixels = data[level];
                for (uint32_t i = 0; i < currentWidth; ++i) {
                    for (uint32_t j = 0; j < currentHeight; ++j) {
                        uint8_t* p = &pixels[4 * (j * currentWidth + i)];
                        if (level == 0) {
                            // Our initial texture formula
                            p[0] = (i / 16) % 2 == (j / 16) % 2 ? 255 : 0; // r
                            p[1] = ((i - j) / 16) % 2 == 0 ? 255 : 0; // g
                            p[2] = ((i + j) / 16) % 2 == 0 ? 255 : 0; // b
                        } else {
                            // Some debug value for visualizing mip levels
                            p[0] = level % 2 == 0 ? 255 : 0;
                            p[1] = (level / 2) % 2 == 0 ? 255 : 0;
                            p[2] = (level / 4) % 2 == 0 ? 255 : 0;
                        }
                        p[3] = 255; // a
                    }
                }
            }
        }

        void write(Context *context, wgpu::Texture underlying) override {
            // Setup copy from data to texture
            wgpu::ImageCopyTexture destination;
            destination.texture = underlying;
            destination.origin = { 0, 0, 0 }; // equivalent of the offset argument of Queue::writeBuffer
            destination.aspect = wgpu::TextureAspect::All; // only relevant for depth/Stencil textures

            // Setup how data is laid out in C++
            wgpu::TextureDataLayout source;
            source.offset = 0;

            uint32_t currentWidth = _width;
            uint32_t currentHeight = _height;
            for (uint32_t level = 0; level < n_mips; ++level) {
                // Create image data for this mip level
                auto &pixels = data[level];

                // Change this to the current level
                destination.mipLevel = level;

                // Compute from the mip level size
                source.bytesPerRow = 4 * currentWidth;
                source.rowsPerImage = currentHeight;

                context->queue.writeTexture(destination, pixels.data(), pixels.size(),
                                                    source,
                                                    {currentWidth, currentHeight, 1});

                // The size of the next mip level:
                // (see https://www.w3.org/TR/webgpu/#logical-miplevel-specific-texture-extent)
                currentWidth /= 2;
                currentHeight /= 2;
            }
        }
    };

    class ImgData : public DataDelegate {
        std::string path;
        int _height;
        int _width;
        unsigned char *pixelData;
    public:
        explicit ImgData(std::string path): path(std::move(path)) {};
        ~ImgData() override = default;

        void initialise(unsigned int &width, unsigned int &height, unsigned int &mips) override {
            int channels;
            pixelData = stbi_load(path.c_str(), &_width, &_height, &channels,
                                                 4 /* force 4 channels */);
            if (nullptr == pixelData)
                throw std::runtime_error("Failed to extract pixel data");

            width = _width;
            height = _height;
            mips = 1;
        }

        void write(Context *context, wgpu::Texture underlying) override {
            // Setup copy from data to texture
            wgpu::ImageCopyTexture destination;
            destination.texture = underlying;
            destination.mipLevel = 0;
            destination.origin = { 0, 0, 0 }; // equivalent of the offset argument of Queue::writeBuffer
            destination.aspect = wgpu::TextureAspect::All; // only relevant for depth/Stencil textures

            // Setup how data is laid out in C++
            wgpu::TextureDataLayout source;
            source.offset = 0;
            source.bytesPerRow = 4 * _width;
            source.rowsPerImage = _height;

            // Generate data and initialise
            context->queue.writeTexture(destination, pixelData,
                                                4 * _width * _height,
                                                source,
                                                {(uint32_t) _width, (uint32_t) _height, 1});

            stbi_image_free(pixelData);
        }
    };

}