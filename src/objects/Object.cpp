#include "objects/Object.hpp"

#include "../webgpu/primitives/buffers/uniforms/ModelMatrixUniform.hpp"
#include "../webgpu/primitives/buffers/AttributeBuffer.hpp"
#include "../webgpu/primitives/buffers/IndexBuffer.hpp"
#include "../webgpu/Scene.hpp"
#include "../webgpu/primitives/textures/Texture2D.hpp"
#include "../State.hpp"
#include "ObjectResources.hpp"


void
tinyrender::Object::setColor(glm::vec3 c) {
    this->resources->color->set(c);
    this->resources->texture = nullptr;

    if(this->targetPipeline != ColoredTriangle) {
        this->targetPipeline = ColoredTriangle;
        this->resources->resetBindGroup(this->context, this->scene, this->targetPipeline);
    }
}

void
tinyrender::Object::setTexture(std::string path) {
    this->resources->texture = std::make_shared<tinyrender::Texture2D::common::BasicImgRepeatingTexture>(context, path);

    if(this->targetPipeline != TexturedTriangle) {
        this->targetPipeline = TexturedTriangle;
        this->resources->resetBindGroup(this->context, this->scene, this->targetPipeline);
    }
}

void tinyrender::Object::setScale(float s)
{
    this->modelMatrix()->setScale(s);
}

void tinyrender::Object::setTranslation(glm::vec3 t)
{
    this->modelMatrix()->setTranslation(t);
}

void tinyrender::Object::setRotationX(float r)
{
    this->modelMatrix()->setRotationX(r);
}

void tinyrender::Object::setRotationY(float r)
{
    this->modelMatrix()->setRotationY(r);
}

void tinyrender::Object::setRotationZ(float r)
{
    this->modelMatrix()->setRotationZ(r);
}

std::shared_ptr<tinyrender::ModelMatrixUniform>
tinyrender::Object::modelMatrix() const
{
    return this->resources->modelMatrix;
}

ObjectPipeline tinyrender::Object::currentTargetPipeline() {
    return targetPipeline;
}

void tinyrender::Object::onInit(Context *c, Scene *s) {
    this->context = c;
    this->scene = s;

    if(this->isInitialised)
        throw std::runtime_error("Object is already initialised");

    this->isInitialised = true;
}

void tinyrender::Object::onUpdate(State &state, float dt)
{
    (void) dt;
    (void) state;
    return;
}

void 
tinyrender::Object::onDraw(wgpu::RenderPassEncoder &renderPass, int vertexBufferSlot, int bindGroupSlot) {
    // Check if set to hidden
    if(HIDDEN)
        return;
    // Object may have no data to draw, which will confuse GPU
    if(this->resources->attributeBuffer->getDrawCalls() == 0)
        return;
    
    // Queue drawing the object
    renderPass.setVertexBuffer(vertexBufferSlot, this->resources->attributeBuffer->getUnderlyingBuffer(), 0, this->resources->attributeBuffer->getSize());
    renderPass.setBindGroup(bindGroupSlot, this->resources->bindGroup, 0, nullptr);
    renderPass.draw(this->resources->attributeBuffer->getDrawCalls(), 1, 0, 0);
}

void 
tinyrender::Object::onRemove() {
    if(!this->isInitialised)
        throw std::runtime_error("Object has not been initialised");

    this->isInitialised = false;
}
