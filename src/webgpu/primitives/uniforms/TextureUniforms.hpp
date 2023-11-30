#pragma once

#include "Uniform.hpp"
#include "../textures/Texture2D.hpp"


namespace tinyrender {

    class TextureUniform : public Uniform {
    protected:
        std::shared_ptr<Texture2D::Texture> texture;
    public:
        TextureUniform(std::shared_ptr<Texture2D::Texture> texture): texture(texture) {};

        wgpu::BindGroupEntry generateUniformBindGroupEntry(int bindGroup) override {
            wgpu::BindGroupEntry entry;
            entry.binding = bindGroup;
            entry.textureView = this->texture->getView();

            return entry;
        };
    };

    class TextureSamplerUniform : public Uniform {
    protected:
        std::shared_ptr<Texture2D::Texture> texture;
    public:
        TextureSamplerUniform(std::shared_ptr<Texture2D::Texture> texture): texture(texture) {};

        wgpu::BindGroupEntry generateUniformBindGroupEntry(int bindGroup) override {
            wgpu::BindGroupEntry entry;
            entry.binding = bindGroup;
            entry.sampler = this->texture->getSampler();

            return entry;
        };
    };

}