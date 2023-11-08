#pragma once

#include <glm/glm.hpp>


struct State {
    uint64_t frame = 0;
    float time = 0;
    glm::vec3 cameraPosition = glm::vec3(0.0, 0.0, 0.0);
};