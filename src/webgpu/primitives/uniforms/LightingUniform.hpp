#pragma once

#include "glm/glm.hpp"
#include "glm/ext.hpp"

#include "BufferUniform.hpp"
#include "UniformStructs.hpp"

using glm::mat4x4;
using glm::vec4;
using glm::vec3;


namespace tinyrender {

    class LightingUniform : public BufferUniform {
    protected:
        LightingUniforms uniforms{};
    public:
        explicit LightingUniform(Context *context);
    };

}
