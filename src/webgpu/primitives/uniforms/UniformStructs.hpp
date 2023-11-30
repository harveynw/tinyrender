#pragma once

#include <array>
#include <glm/glm.hpp>

using glm::mat4x4;
using glm::vec4;
using glm::vec3;

struct ViewProjectionUniforms {
    mat4x4 projectionMatrix;
    mat4x4 viewMatrix;
    vec3 cameraWorldPosition;
    float _pad;
};
static_assert(sizeof(ViewProjectionUniforms) % 16 == 0, "Must be multiple of 16 bytes");

struct ModelUniform {
    mat4x4 modelMatrix;
};
static_assert(sizeof(ModelUniform) % 16 == 0, "Must be multiple of 16 bytes");

struct LightingUniforms {
    std::array<glm::vec4, 2> directions;
    std::array<glm::vec4, 2> colors;
    float ambient;
    float _pad[3];
};
static_assert(sizeof(LightingUniforms) % 16 == 0);

struct ColorUniforms {
    glm::vec3 color;
    float _pad;
};
static_assert(sizeof(ColorUniforms) % 16 == 0);

struct ScalarUniforms {
    float scalar;
    float _pad[3];
};
static_assert(sizeof(ScalarUniforms) % 16 == 0);
