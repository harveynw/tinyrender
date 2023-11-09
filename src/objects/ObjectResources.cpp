#include "ObjectResources.hpp"


ObjectResources::ObjectResources(Context *context, Scene *scene, std::shared_ptr<tinyrender::AttributeBuffer> attrs, ObjectPipeline target) {
    this->attributeBuffer = std::move(attrs);
    this->modelMatrix = std::make_shared<tinyrender::ModelMatrixUniform>(context);
    this->color = std::make_shared<tinyrender::ColorUniform>(context);

    this->resetBindGroup(context, scene, target);
}

ObjectResources::~ObjectResources() {
    if(bindGroup != nullptr)
        bindGroup.release();
}

void
ObjectResources::resetBindGroup(Context *context, Scene *scene, ObjectPipeline target) {
    if(bindGroup != nullptr) {
        bindGroup.release();
    }

    switch(target) {
        case ColoredTriangle: {
            bindGroupData = std::vector<BindGroupEntry>(2);
            bindGroupData[0] = this->modelMatrix->generateUniformBindGroupEntry(0);
            bindGroupData[1] = this->color->generateUniformBindGroupEntry(1);

            BindGroupDescriptor bindGroupDesc;
            bindGroupDesc.layout = scene->coloredShader->objectBindGroupLayout(); // Point to WGPUBindGroupLayout
            bindGroupDesc.entryCount = (uint32_t)bindGroupData.size();
            bindGroupDesc.entries = bindGroupData.data();
            bindGroup = context->device.createBindGroup(bindGroupDesc);
            break;
        }
        case TexturedTriangle: {
            bindGroupData = std::vector<BindGroupEntry>(3);
            bindGroupData[0] = this->modelMatrix->generateUniformBindGroupEntry(0);
            bindGroupData[1].binding = 1;
            bindGroupData[1].textureView = this->texture->getView();
            bindGroupData[2].binding = 2;
            bindGroupData[2].sampler = this->texture->getSampler();

            BindGroupDescriptor bindGroupDesc;
            bindGroupDesc.layout = scene->texturedShader->objectBindGroupLayout();
            bindGroupDesc.entryCount = (uint32_t)bindGroupData.size();
            bindGroupDesc.entries = bindGroupData.data();
            bindGroup = context->device.createBindGroup(bindGroupDesc);
            break;
        }
        case Waves: {
            bindGroupData = std::vector<BindGroupEntry>(5);
            bindGroupData[0] = this->modelMatrix->generateUniformBindGroupEntry(0);
            bindGroupData[1].binding = 1;
            bindGroupData[1].textureView =  this->texture->getView();
            bindGroupData[2].binding = 2;
            bindGroupData[2].sampler = this->texture->getSampler();
            bindGroupData[3] = this->maxDisplacement->generateUniformBindGroupEntry(3);
            bindGroupData[4] = this->color->generateUniformBindGroupEntry(4);

            BindGroupDescriptor bindGroupDesc;
            bindGroupDesc.layout = scene->wavesShader->objectBindGroupLayout();
            bindGroupDesc.entryCount = (uint32_t)bindGroupData.size();
            bindGroupDesc.entries = bindGroupData.data();
            bindGroup = context->device.createBindGroup(bindGroupDesc);
            break;
        }
    }
}

