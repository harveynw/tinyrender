#pragma once

#include "glm/glm.hpp"
#include "glm/ext.hpp"

#include "../Buffer.hpp"

using glm::mat4x4;
using glm::vec4;
using glm::vec3;

namespace {
    struct LightingUniforms {
        std::array<glm::vec4, 2> directions;
        std::array<glm::vec4, 2> colors;
    };
    static_assert(sizeof(LightingUniforms) % 16 == 0);
}

namespace engine {

    class LightingUniform : public Buffer {
    protected:
        LightingUniforms uniforms{};
    public:
        explicit LightingUniform(Context *context);

        static int minBindingSize() { return sizeof(LightingUniforms); };
    };

}
