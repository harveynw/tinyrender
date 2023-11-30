#pragma once

#include "Shader.hpp"

namespace tinyrender {

    class VoxelShader : public Shader {
    protected:
        Context *context;

        wgpu::FragmentState fragmentState;
        wgpu::BlendState blendState;
        wgpu::ColorTargetState colorTarget;

        std::vector<VertexAttribute> vertexAttribs;
        VertexBufferLayout vertexBufferLayout;
    public:
        explicit VoxelShader(Context *context);
        ~VoxelShader() = default;

        wgpu::BindGroupLayout viewProjBindGroupLayout() override;
        wgpu::BindGroupLayout objectBindGroupLayout() override;

        void setAsTarget(RenderPipelineDescriptor &desc) override;
    };

}