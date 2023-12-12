#pragma once

#include "glm/glm.hpp"
#include "glm/ext.hpp"

#include "BufferUniform.hpp"
#include "UniformStructs.hpp"

using glm::mat4x4;
using glm::vec4;
using glm::vec3;


class ModelMatrixUniform : public BufferUniform {
protected:
    ModelUniform uniforms{};

    float scale = 1.0;
    vec3 translate = vec3(0, 0, 0);
    float rotateX = 0.0;
    float rotateY = 0.0;
    float rotateZ = 0.0;

    mat4x4 computeModelMatrix();
    void updateBuffer();
public:
    explicit ModelMatrixUniform(Context *context);

    void setScale(float s) { scale = s; updateBuffer(); };
    void setTranslation(vec3 t) { translate = t; updateBuffer(); };
    void setRotationX(float r) { rotateX = r; updateBuffer(); };
    void setRotationY(float r) { rotateY = r; updateBuffer(); };
    void setRotationZ(float r) { rotateZ = r; updateBuffer(); };
};

