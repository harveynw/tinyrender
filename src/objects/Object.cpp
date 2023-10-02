#include "Object.hpp"

#include <utility>

void
engine::Object::setColor(glm::vec3 c) {
    this->resources->color->set(c);
    this->resources->texture = nullptr;

    if(this->targetPipeline != ColoredTriangle) {
        this->targetPipeline = ColoredTriangle;
        this->resources->resetBindGroup(this->context, this->scene, this->targetPipeline);
    }
}

void
engine::Object::setTexture(std::shared_ptr<engine::Texture2D::Texture> texture) {
    this->resources->texture = std::move(texture);

    if(this->targetPipeline != TexturedTriangle) {
        this->targetPipeline = TexturedTriangle;
        this->resources->resetBindGroup(this->context, this->scene, this->targetPipeline);
    }
}

std::shared_ptr<engine::ModelMatrixUniform>
engine::Object::modelMatrix() const {
    return this->resources->modelMatrix;
}

ObjectPipeline engine::Object::currentTargetPipeline() {
    return targetPipeline;
}

