#include "Scene.hpp"

Scene::Scene(std::shared_ptr<Context> context) {
    this->buildDepthBuffer(context);

    // Init shaders
    shaders[TexturedTriangle] = std::make_shared<TexturedShader>(context.get());
    shaders[ColoredTriangle] = std::make_shared<ColoredShader>(context.get());
    shaders[Waves] = std::make_shared<WavesShader>(context.get());
    shaders[Voxels] = std::make_shared<VoxelShader>(context.get());

    // View Projection + Lighting Uniform (present in every shader)
    this->viewProjUniform = std::make_shared<ViewProjMatrixUniform>(context.get(), vec3(5.0, -5.0, 5.0), vec3(0, 0, 0));
    this->lightingUniform = std::make_shared<LightingUniform>(context.get());

    // Build bind groups for each shader
    viewProjUniformBindGroupData = std::vector<BindGroupEntry>(2);
    viewProjUniformBindGroupData[0] = viewProjUniform->generateUniformBindGroupEntry(0);
    viewProjUniformBindGroupData[1] = lightingUniform->generateUniformBindGroupEntry(1);

    for(auto entry : shaders) {
        ObjectPipeline pipeline = entry.first;
        std::shared_ptr<Shader> shader = entry.second;

        assert(shader != nullptr);

        BindGroupDescriptor bindGroupDesc;
        bindGroupDesc.entryCount = (uint32_t) viewProjUniformBindGroupData.size();
        bindGroupDesc.entries = viewProjUniformBindGroupData.data();
        bindGroupDesc.layout = shader->viewProjBindGroupLayout();
        bindGroups.at(pipeline) = context->device.createBindGroup(bindGroupDesc);
    }
}

void Scene::buildDepthBuffer(const std::shared_ptr<Context>& context) {
    depthTexture = std::make_shared<Texture2D::common::DefaultDepthTexture>(context.get());
}