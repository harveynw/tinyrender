#pragma once

#include "../../../engine.hpp"

namespace engine::Texture2D {
    class SamplerDelegate {
    public:
        virtual wgpu::Sampler createSampler(Engine *engine) = 0;
        virtual ~SamplerDelegate() = default;
    };

    class NoSampler : public SamplerDelegate {
    public:
        ~NoSampler() override = default;

        wgpu::Sampler createSampler(Engine *engine) override {
            (void) engine;
            return nullptr;
        }
    };

    class DefaultSampler : public SamplerDelegate {
    public:
        ~DefaultSampler() override = default;

        wgpu::Sampler createSampler(Engine *engine) override {
            SamplerDescriptor samplerDesc;
            samplerDesc.addressModeU = AddressMode::MirrorRepeat; //AddressMode::ClampToEdge;
            samplerDesc.addressModeV = AddressMode::MirrorRepeat;
            samplerDesc.addressModeW = AddressMode::ClampToEdge;
            samplerDesc.magFilter = FilterMode::Linear;
            samplerDesc.minFilter = FilterMode::Linear;
            samplerDesc.mipmapFilter = MipmapFilterMode::Linear;
            samplerDesc.lodMinClamp = 0.0f;
            samplerDesc.lodMaxClamp = 1.0f;
            samplerDesc.compare = CompareFunction::Undefined;
            samplerDesc.maxAnisotropy = 1;

            return engine->wgpuGetDevice().createSampler(samplerDesc);
        }
    };
}