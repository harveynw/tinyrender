//
// Created by Harvey Williams on 09/08/2023.
//

#ifndef TINYGAME_UNIFORMMVPBUFFER_H
#define TINYGAME_UNIFORMMVPBUFFER_H

#include "glm/glm.hpp"
#include "glm/ext.hpp"

#include "../Buffer.hpp"
#include "UniformStructs.hpp"

using glm::mat4x4;
using glm::vec4;
using glm::vec3;

namespace engine {

    class ViewProjMatrixUniform : public Buffer {
    protected:
        ViewProjectionUniforms uniforms{};

        // Geometric properties
        // TODO: a lot of these can be deleted and be managed by Controller/Camera class
        vec3 lookFrom;
        vec3 lookAt;
        float focalLength;
        float near;
        float far;
        float fov;
    public:
        ViewProjMatrixUniform(Context *context, vec3 lookFrom, vec3 lookAt);

        void refreshProjectionMatrix(Context *context); // Useful if dimensions of window change
        void updateViewMatrix(glm::mat4x4 viewMatrix);
        void updateCameraPosition(glm::vec3 cameraWorldPosition);
    };

}

#endif //TINYGAME_UNIFORMMVPBUFFER_H
