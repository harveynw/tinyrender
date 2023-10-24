//
// Created by Harvey Williams on 09/08/2023.
//

#include "ViewProjMatrixUniform.hpp"

engine::ViewProjMatrixUniform::ViewProjMatrixUniform(Context *context, vec3 lookFrom, vec3 lookAt): Buffer() {
    this->lookFrom = lookFrom;
    this->lookAt = lookAt;

    // Params
    this->focalLength = 2.0;
    this->near = 0.001f;
    this->far = 100.0f;
    float ratio = context->DISPLAY_WIDTH / context->DISPLAY_HEIGHT;
    this->fov = 2 * glm::atan(1 / focalLength);
    uniforms.viewMatrix = glm::lookAt(lookFrom, lookAt, vec3(0, 0, 1));
    uniforms.projectionMatrix = glm::perspective(fov, ratio, near, far);
    uniforms.cameraWorldPosition = this->lookFrom;

    // Buffer base class is initialised
    this->context = context;
    this->type = engine::UNIFORM;
    this->size = sizeof(ViewProjectionUniforms);
    this->mapped = false;
    this->initialise(&uniforms);
}

void
engine::ViewProjMatrixUniform::updateViewMatrix(glm::mat4x4 viewMatrix) {
    uniforms.viewMatrix = viewMatrix;
    context->queue.writeBuffer(underlying, offsetof(ViewProjectionUniforms, viewMatrix),
            &uniforms.viewMatrix, sizeof(ViewProjectionUniforms::viewMatrix));
}

void engine::ViewProjMatrixUniform::updateCameraPosition(glm::vec3 cameraWorldPosition)
{
    uniforms.cameraWorldPosition = cameraWorldPosition;
    context->queue.writeBuffer(underlying, offsetof(ViewProjectionUniforms, cameraWorldPosition),
            &uniforms.cameraWorldPosition, sizeof(ViewProjectionUniforms::cameraWorldPosition));
}

void engine::ViewProjMatrixUniform::refreshProjectionMatrix(Context *context) {
    float ratio = context->DISPLAY_WIDTH / context->DISPLAY_HEIGHT;
    uniforms.projectionMatrix = glm::perspective(fov, ratio, near, far);
    context->queue.writeBuffer(underlying, offsetof(ViewProjectionUniforms, projectionMatrix),
                               &uniforms.projectionMatrix,
                               sizeof(ViewProjectionUniforms::projectionMatrix));
}
