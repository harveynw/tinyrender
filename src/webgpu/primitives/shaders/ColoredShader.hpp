#pragma once

#include "Shader.hpp"


class ColoredShader : public Shader {
protected:
    Context *context;

    wgpu::FragmentState fragmentState;
    wgpu::BlendState blendState;
    wgpu::ColorTargetState colorTarget;

    std::vector<VertexAttribute> vertexAttribs;
    VertexBufferLayout vertexBufferLayout;
public:
    explicit ColoredShader(Context *context);
    ~ColoredShader() = default;

    wgpu::BindGroupLayout viewProjBindGroupLayout() override;
    wgpu::BindGroupLayout objectBindGroupLayout() override;

    void setAsTarget(RenderPipelineDescriptor &desc) override;
};
