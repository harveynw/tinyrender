#pragma once

#include "../../../engine.hpp"

namespace engine::Texture2D {
    class SamplerDelegate {
    public:
        virtual wgpu::Sampler createSampler(Engine *engine, unsigned int mips) = 0;

        virtual ~SamplerDelegate() = default;
    };

    class NoSampler : public SamplerDelegate {
    public:
        ~NoSampler() override = default;

        wgpu::Sampler createSampler(Engine *engine, unsigned int mips) override {
            (void) engine;
            (void) mips;
            return nullptr;
        }
    };

    class DefaultSampler : public SamplerDelegate {
    public:
        ~DefaultSampler() override = default;

        wgpu::Sampler createSampler(Engine *engine, unsigned int mips) override {
            SamplerDescriptor samplerDesc;
            samplerDesc.addressModeU = AddressMode::MirrorRepeat; //AddressMode::ClampToEdge;
            samplerDesc.addressModeV = AddressMode::MirrorRepeat;
            samplerDesc.addressModeW = AddressMode::ClampToEdge;
            samplerDesc.magFilter = FilterMode::Linear;
            samplerDesc.minFilter = FilterMode::Linear;
            samplerDesc.mipmapFilter = MipmapFilterMode::Linear;
            samplerDesc.lodMinClamp = 0.0f;
            samplerDesc.lodMaxClamp = (float) mips; // {1.0, 8.0}
            samplerDesc.compare = CompareFunction::Undefined;
            samplerDesc.maxAnisotropy = 1;

            return engine->wgpuGetDevice().createSampler(samplerDesc);
        }
    };
}