#include "ModelMatrixUniform.hpp"

tinyrender::ModelMatrixUniform::ModelMatrixUniform(Context *context) {
    uniforms.modelMatrix = computeModelMatrix();

    // Buffer base class is initialised
    this->context = context;
    this->type = tinyrender::UNIFORM;
    this->size = sizeof(ModelUniform);
    this->mapped = false;
    this->initialise(&uniforms);
}

mat4x4 tinyrender::ModelMatrixUniform::computeModelMatrix() {
    mat4x4 S, T, R;
    S = glm::scale(mat4x4(1.0), vec3(scale));
    T = glm::translate(mat4x4(1.0), translate);
    R = mat4x4(1.0);
    R = glm::rotate(R, rotateX, vec3(1.0, 0.0, 0.0));
    R = glm::rotate(R, rotateY, vec3(0.0, 1.0, 0.0));
    R = glm::rotate(R, rotateZ, vec3(0.0, 0.0, 1.0));
    return T * R * S;
}

void tinyrender::ModelMatrixUniform::updateBuffer() {
    uniforms.modelMatrix = computeModelMatrix();
    context->queue.writeBuffer(underlying, offsetof(ModelUniform, modelMatrix),
                                       &uniforms.modelMatrix,sizeof(ModelUniform::modelMatrix));
}

