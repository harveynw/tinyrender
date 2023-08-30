//
// Created by Harvey Williams on 09/08/2023.
//

#include "UniformViewProjectionBuffer.hpp"

engine::UniformViewProjectionBuffer::UniformViewProjectionBuffer(Engine *engine, vec3 lookFrom, vec3 lookAt): Buffer() {
    // Params
    float focalLength = 2.0;
    uniforms.viewMatrix = glm::lookAt(lookFrom, lookAt, vec3(0, 0, 1));
    float near = 0.001f;
    float far = 100.0f;
    float ratio = engine->DISPLAY_WIDTH / engine->DISPLAY_HEIGHT;
    float fov = 2 * glm::atan(1 / focalLength);
    uniforms.projectionMatrix = glm::perspective(fov, ratio, near, far);
    uniforms.time = 1.0f;

    // Buffer base class is initialised
    this->engine = engine;
    this->type = engine::UNIFORM;
    this->size = sizeof(engine::ViewProjectionUniforms);
    this->mapped = false;
    this->initialise(&uniforms);
}

void
engine::UniformViewProjectionBuffer::updateRotationAboutZAxis() {
    uniforms.time = static_cast<float>(glfwGetTime()); // glfwGetTime returns a double
    // Only update the 1-st float of the buffer
    engine->wgpuGetQueue().writeBuffer(underlying, offsetof(engine::ViewProjectionUniforms, time),
                              &uniforms.time,sizeof(engine::ViewProjectionUniforms::time));

    // Rotate camera around z-axis
    mat4x4 newViewMatrix= glm::rotate(uniforms.viewMatrix, 2*glm::sin(uniforms.time),
                                      vec3(0.0, 0.0, 1.0));
    engine->wgpuGetQueue().writeBuffer(underlying, offsetof(engine::ViewProjectionUniforms, viewMatrix),
                              &newViewMatrix, sizeof(engine::ViewProjectionUniforms::viewMatrix));
}

void
engine::UniformViewProjectionBuffer::updateViewMatrix(glm::mat4x4 viewMatrix) {
    uniforms.viewMatrix = viewMatrix;
    engine->wgpuGetQueue().writeBuffer(underlying, offsetof(engine::ViewProjectionUniforms, viewMatrix),
                              &uniforms.viewMatrix,sizeof(engine::ViewProjectionUniforms::viewMatrix));
}
