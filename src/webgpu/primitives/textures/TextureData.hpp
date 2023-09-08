#pragma once

#include <utility>
#include <webgpu/webgpu.hpp>
#include <filesystem>
#include "stb_image.h"

#include "../../../engine.hpp"

#ifdef __APPLE__
namespace fs = std::__fs::filesystem;
#else
namespace fs = std::filesystem;
#endif



namespace engine::Texture2D {

    class DataDelegate {
    public:
        virtual ~DataDelegate() = default;

        // Initialise populates width and height so the texture knows what size it is
        virtual void initialise(unsigned int &width, unsigned int &height) = 0;
        // Write, to be called after initialise, is given the underlying texture to target and populate in GPU memory
        virtual void write(Engine *engine, wgpu::Texture underlying) = 0;
    };

    class NoData : public DataDelegate {
    public:
        ~NoData() override = default;

        void initialise(unsigned int &width, unsigned int &height) override {
            (void) width;
            (void) height;
            // no-op
        }
        void write(Engine *engine, wgpu::Texture underlying) override {
            (void) engine;
            (void) underlying;
            // no-op
        }
    };

    class DebugData : public DataDelegate {
    protected:
        unsigned int _width;
        unsigned int _height;
        std::vector<uint8_t> data;
    public:
        DebugData(unsigned int width, unsigned int height): _width(width), _height(height) {};
        ~DebugData() override = default;

        void initialise(unsigned int &width, unsigned int &height) override {
            width = this->_width;
            height = this->_height;

            // Generate data and initialise
            data = std::vector<uint8_t>(4 * width * height);
            for (uint32_t i = 0; i < width; ++i) {
                for (uint32_t j = 0; j < height; ++j) {
                    uint8_t *p = &data[4 * (j * width + i)];

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

        void write(Engine *engine, wgpu::Texture underlying) override {
            // Setup copy from data to texture
            wgpu::ImageCopyTexture destination;
            destination.texture = underlying;
            destination.mipLevel = 0;
            destination.origin = { 0, 0, 0 }; // equivalent of the offset argument of Queue::writeBuffer
            destination.aspect = TextureAspect::All; // only relevant for depth/Stencil textures

            // Setup how data is laid out in C++
            wgpu::TextureDataLayout source;
            source.offset = 0;
            source.bytesPerRow = 4 * _width;
            source.rowsPerImage = _height;

            engine->wgpuGetQueue().writeTexture(destination, data.data(), data.size(), source,
                                                {_width, _height, 1});
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

        void initialise(unsigned int &width, unsigned int &height) override {
            int channels;
            pixelData = stbi_load(path.c_str(), &_width, &_height, &channels,
                                                 4 /* force 4 channels */);
            if (nullptr == pixelData)
                throw std::runtime_error("Failed to extract pixel data");

            width = _width;
            height = _height;
        }

        void write(Engine *engine, wgpu::Texture underlying) override {
            // Setup copy from data to texture
            wgpu::ImageCopyTexture destination;
            destination.texture = underlying;
            destination.mipLevel = 0;
            destination.origin = { 0, 0, 0 }; // equivalent of the offset argument of Queue::writeBuffer
            destination.aspect = TextureAspect::All; // only relevant for depth/Stencil textures

            // Setup how data is laid out in C++
            wgpu::TextureDataLayout source;
            source.offset = 0;
            source.bytesPerRow = 4 * _width;
            source.rowsPerImage = _height;

            // Generate data and initialise
            engine->wgpuGetQueue().writeTexture(destination, pixelData,
                                                4 * _width * _height,
                                                source,
                                                {(uint32_t) _width, (uint32_t) _height, 1});

            stbi_image_free(pixelData);
        }
    };

}