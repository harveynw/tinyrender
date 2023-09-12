//
// Created by Harvey Williams on 09/08/2023.
//

#pragma once

#include "glm/glm.hpp"
#include "glm/ext.hpp"

#include "../Buffer.hpp"

using glm::mat4x4;
using glm::vec4;
using glm::vec3;

namespace engine {

    struct ModelUniform {
        mat4x4 modelMatrix;
    };
    static_assert(sizeof(ModelUniform) % 16 == 0, "Must be multiple of 16 bytes");


    class ModelMatrixUniform : public Buffer {
    protected:
        ModelUniform uniforms{};

        float scale = 1.0;
        vec3 translate = vec3(0, 0, 0);
        float rotate = 0.0;

        mat4x4 computeModelMatrix();
        void updateBuffer();
    public:
        explicit ModelMatrixUniform(Context *context);

        void setScale(float s) { scale = s; updateBuffer(); };
        void setTranslation(vec3 t) { translate = t; updateBuffer(); };
        void setRotation(float r) { rotate = r; updateBuffer(); };
    };

}
