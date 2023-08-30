//
// Created by Harvey Williams on 17/08/2023.
//
#include "UniformModelBuffer.hpp"

engine::UniformModelBuffer::UniformModelBuffer(Engine *engine) {
    uniforms.modelMatrix = computeModelMatrix();

    // Buffer base class is initialised
    this->engine = engine;
    this->type = engine::UNIFORM;
    this->size = sizeof(engine::ModelUniform);
    this->mapped = false;
    this->initialise(&uniforms);
}

mat4x4 engine::UniformModelBuffer::computeModelMatrix() {
    mat4x4 S, T, R;
    S = glm::scale(mat4x4(1.0), vec3(scale));
    T = glm::translate(mat4x4(1.0), translate);
    R = glm::rotate(mat4x4(1.0), rotate, vec3(0.0, 0.0, 1.0));
    //return R * T * S;
    return T * R * S;
}

void engine::UniformModelBuffer::updateBuffer() {
    uniforms.modelMatrix = computeModelMatrix();
    engine->wgpuGetQueue().writeBuffer(underlying, offsetof(engine::ModelUniform, modelMatrix),
                                       &uniforms.modelMatrix,sizeof(engine::ModelUniform::modelMatrix));
}

