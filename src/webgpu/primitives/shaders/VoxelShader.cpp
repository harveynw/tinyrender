#include "VoxelShader.hpp"


tinyrender::VoxelShader::VoxelShader(Context *context): context(context) {
    // Create shader module
    shaderModule = loadShaderModule("voxels", context->device);
    std::cout << "Shader module: " << shaderModule << std::endl;

    // Create bind groups
    bindGroupLayouts = { sceneUniformLayout(context), voxelUniformLayout(context) };

    // Create layouts
    PipelineLayoutDescriptor layoutDesc{};
    layoutDesc.bindGroupLayoutCount = 2;
    layoutDesc.bindGroupLayouts = (WGPUBindGroupLayout*) bindGroupLayouts.data();
    layout = context->device.createPipelineLayout(layoutDesc);

    // Create vertex attributes
    vertexAttribs = std::vector<VertexAttribute>(2);
    // Position attribute
    vertexAttribs[0].shaderLocation = 0;
    vertexAttribs[0].format = VertexFormat::Float32x3;
    vertexAttribs[0].offset = offsetof(VoxelVertexAttribute, position);
    // Data attribute
    vertexAttribs[1].shaderLocation = 1;
    vertexAttribs[1].format = VertexFormat::Float32;
    vertexAttribs[1].offset = offsetof(VoxelVertexAttribute, data);
    // Single vertex buffer layout 
    vertexBufferLayout.attributeCount = (uint32_t)vertexAttribs.size();
    vertexBufferLayout.attributes = vertexAttribs.data();
    vertexBufferLayout.arrayStride = sizeof(VoxelVertexAttribute);
    vertexBufferLayout.stepMode = VertexStepMode::Vertex;
}

wgpu::BindGroupLayout
tinyrender::VoxelShader::objectBindGroupLayout() {
    return bindGroupLayouts[1];
}

wgpu::BindGroupLayout
tinyrender::VoxelShader::viewProjBindGroupLayout() {
    return bindGroupLayouts[0];
}

void
tinyrender::VoxelShader::setAsTarget(RenderPipelineDescriptor &desc) {
    // Attributes
    // Specify that we have one buffer and this is the layout
    desc.vertex.bufferCount = 1;
    desc.vertex.buffers = &vertexBufferLayout;

    // Vertex shader
    desc.vertex.module = shaderModule;
    desc.vertex.entryPoint = "vs_main";
    desc.vertex.constantCount = 0;
    desc.vertex.constants = nullptr;

    // Fragment shader
    desc.fragment = &fragmentState;
    fragmentState.module = shaderModule;
    fragmentState.entryPoint = "fs_main";
    fragmentState.constantCount = 0;
    fragmentState.constants = nullptr;

    // Configure blend state
    // Usual alpha blending for the color:
    blendState.color.srcFactor = BlendFactor::SrcAlpha;
    blendState.color.dstFactor = BlendFactor::OneMinusSrcAlpha;
    blendState.color.operation = BlendOperation::Add;
    // We leave the target alpha untouched:
    blendState.alpha.srcFactor = BlendFactor::Zero;
    blendState.alpha.dstFactor = BlendFactor::One;
    blendState.alpha.operation = BlendOperation::Add;

    colorTarget.format = context->swapChainFormat;
    colorTarget.blend = &blendState;
    colorTarget.writeMask = ColorWriteMask::All; // We could write to only some of the color channels.

    // We have only one target because our render pass has only one output color
    // attachment.
    fragmentState.targetCount = 1;
    fragmentState.targets = &colorTarget;
}
