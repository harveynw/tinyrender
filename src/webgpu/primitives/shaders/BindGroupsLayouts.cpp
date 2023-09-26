#include "BindGroupsLayouts.hpp"


wgpu::BindGroupLayout
sceneUniformLayout(Context *context) {
    std::vector<wgpu::BindGroupLayoutEntry> entries(2, wgpu::Default);
    wgpu::BindGroupLayoutDescriptor bindGroupLayoutDesc{};

    /*
     * Scene: Lighting + View*Projection Matrix
     */
    wgpu::BindGroupLayoutEntry& viewProjectionBindingLayout = entries[0];
    viewProjectionBindingLayout.binding = 0;
    viewProjectionBindingLayout.visibility = wgpu::ShaderStage::Vertex | wgpu::ShaderStage::Fragment;
    viewProjectionBindingLayout.buffer.type = wgpu::BufferBindingType::Uniform;
    viewProjectionBindingLayout.buffer.minBindingSize = sizeof(ViewProjectionUniforms);

    wgpu::BindGroupLayoutEntry& lightingBindingLayout = entries[1];
    lightingBindingLayout.binding = 1;
    lightingBindingLayout.visibility = wgpu::ShaderStage::Fragment;
    lightingBindingLayout.buffer.type = wgpu::BufferBindingType::Uniform;
    lightingBindingLayout.buffer.minBindingSize = sizeof(LightingUniforms);

    bindGroupLayoutDesc.entryCount = (uint32_t) entries.size();
    bindGroupLayoutDesc.entries = entries.data();
    return context->device.createBindGroupLayout(bindGroupLayoutDesc);
}

wgpu::BindGroupLayout
texturedModelUniformLayout(Context *context) {
    std::vector<wgpu::BindGroupLayoutEntry> entries(3, wgpu::Default);
    wgpu::BindGroupLayoutDescriptor bindGroupLayoutDesc{};

    /*
     * Textured Model: Model matrix + Texture + Sampler
     */
    wgpu::BindGroupLayoutEntry& modelBindingLayout = entries[0];
    modelBindingLayout.binding = 0;
    modelBindingLayout.visibility = wgpu::ShaderStage::Vertex | wgpu::ShaderStage::Fragment;
    modelBindingLayout.buffer.type = wgpu::BufferBindingType::Uniform;
    modelBindingLayout.buffer.minBindingSize = sizeof(ModelUniform);

    wgpu::BindGroupLayoutEntry& textureBindingLayout = entries[1];
    textureBindingLayout.binding = 1;
    textureBindingLayout.visibility = wgpu::ShaderStage::Fragment;
    textureBindingLayout.texture.sampleType = wgpu::TextureSampleType::Float; // Because using normalised format
    textureBindingLayout.texture.viewDimension = wgpu::TextureViewDimension::_2D;

    wgpu::BindGroupLayoutEntry& samplerBindingLayout = entries[2];
    samplerBindingLayout.binding = 2;
    samplerBindingLayout.visibility = wgpu::ShaderStage::Fragment;
    samplerBindingLayout.sampler.type = wgpu::SamplerBindingType::Filtering;

    bindGroupLayoutDesc.entryCount = (uint32_t) entries.size();
    bindGroupLayoutDesc.entries = entries.data();
    return context->device.createBindGroupLayout(bindGroupLayoutDesc);
}

wgpu::BindGroupLayout
coloredModelUniformLayout(Context *context) {
    std::vector<wgpu::BindGroupLayoutEntry> entries(2, wgpu::Default);
    wgpu::BindGroupLayoutDescriptor bindGroupLayoutDesc{};

    /*
     * Colored Model: Model matrix + color
     */

    wgpu::BindGroupLayoutEntry &modelBindingLayout2 = entries[0];
    modelBindingLayout2.binding = 0;
    modelBindingLayout2.visibility = wgpu::ShaderStage::Vertex | wgpu::ShaderStage::Fragment;
    modelBindingLayout2.buffer.type = wgpu::BufferBindingType::Uniform;
    modelBindingLayout2.buffer.minBindingSize = sizeof(ModelUniform);

    wgpu::BindGroupLayoutEntry &colorBindingLayout = entries[1];
    colorBindingLayout.binding = 1;
    colorBindingLayout.visibility = wgpu::ShaderStage::Fragment;
    colorBindingLayout.buffer.type = wgpu::BufferBindingType::Uniform;
    colorBindingLayout.buffer.minBindingSize = sizeof(glm::vec3);

    bindGroupLayoutDesc.entryCount = (uint32_t) entries.size();
    bindGroupLayoutDesc.entries = entries.data();
    return context->device.createBindGroupLayout(bindGroupLayoutDesc);
}
