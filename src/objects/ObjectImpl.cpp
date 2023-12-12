#include "ObjectImpl.hpp"


void
ObjectImpl::setColor(glm::vec3 c) {
    resources->color->set(c);
    resources->texture = nullptr;

    if(resources->targetPipeline != ColoredTriangle)
        resources->resetBindGroup(ColoredTriangle);
}

void
ObjectImpl::setTexture(std::string path) {
    resources->texture = std::make_shared<Texture2D::common::BasicImgRepeatingTexture>(context, path);

    if(resources->targetPipeline != TexturedTriangle)
        resources->resetBindGroup(TexturedTriangle);
}

void ObjectImpl::setScale(float s)
{
    this->modelMatrix()->setScale(s);
}

void ObjectImpl::setTranslation(glm::vec3 t)
{
    this->modelMatrix()->setTranslation(t);
}

void ObjectImpl::setRotationX(float r)
{
    this->modelMatrix()->setRotationX(r);
}

void ObjectImpl::setRotationY(float r)
{
    this->modelMatrix()->setRotationY(r);
}

void ObjectImpl::setRotationZ(float r)
{
    this->modelMatrix()->setRotationZ(r);
}

std::shared_ptr<ModelMatrixUniform>
ObjectImpl::modelMatrix() const
{
    return this->resources->modelMatrix;
}

ObjectPipeline ObjectImpl::currentTargetPipeline() {
    return resources->targetPipeline;
}

void ObjectImpl::onInit(Context *c, Scene *s) {
    this->context = c;
    this->scene = s;

    if(this->isInitialised)
        throw std::runtime_error("Object is already initialised");

    this->isInitialised = true;
}

void ObjectImpl::onUpdate(State &state, float dt)
{
    (void) dt;
    (void) state;
    return;
}

void 
ObjectImpl::onDraw(wgpu::RenderPassEncoder &renderPass, int vertexBufferSlot, int bindGroupSlot) {
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
ObjectImpl::onRemove() {
    if(!this->isInitialised)
        throw std::runtime_error("Object has not been initialised");

    this->isInitialised = false;
}

tinyrender::Object::Object() = default;
tinyrender::Object::~Object() = default;

void 
tinyrender::Object::setColor(glm::vec3 c) {
    obj->setColor(c);
}

void 
tinyrender::Object::setTexture(std::string path) {
    obj->setTexture(path);
}

void 
tinyrender::Object::setScale(float s) {
    obj->setScale(s);
}

void 
tinyrender::Object::setTranslation(glm::vec3 t) {
    obj->setTranslation(t);
}

void 
tinyrender::Object::setRotationX(float r) {
    obj->setRotationX(r);
}

void 
tinyrender::Object::setRotationY(float r) {
    obj->setRotationY(r);
}

void 
tinyrender::Object::setRotationZ(float r) {
    obj->setRotationZ(r);
}


void 
tinyrender::Object::setVisibility(char visibility) {
    if(visibility == tinyrender::OBJECT_VISIBLE)
        obj->HIDDEN = false;
    else if (visibility == tinyrender::OBJECT_HIDDEN)
        obj->HIDDEN = true;
    else
        throw std::runtime_error("Wrong visibility parameter: " + std::string(1, visibility));
}