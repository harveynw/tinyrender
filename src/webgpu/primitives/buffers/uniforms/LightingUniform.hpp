#pragma once

#include "glm/glm.hpp"
#include "glm/ext.hpp"

#include "../Buffer.hpp"
#include "UniformStructs.hpp"

using glm::mat4x4;
using glm::vec4;
using glm::vec3;


namespace engine {

    class LightingUniform : public Buffer {
    protected:
        LightingUniforms uniforms{};
    public:
        explicit LightingUniform(Context *context);
    };

}
