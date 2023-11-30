#pragma once

#include <algorithm>
#include <glm/glm.hpp>

#include "Fwd.hpp"


namespace tinyrender {
    class Transform {
    protected:
        glm::mat4 t;
    public:
        Transform() {
            t = glm::mat4(1.0f);
        }
        Transform(glm::mat4 _t) {
            t = _t;
        }
        ~Transform() = default;  

        Transform(const Transform &transform): t(transform.t) {}; // copy constructor
        Transform(Transform &&other) noexcept: t(std::exchange(other.t, glm::mat4(1.0f))) {}; // move constructor
        Transform& operator=(const Transform &other) { // copy assignment
            return *this = Transform(other);
        };
        Transform& operator=(Transform &&other) noexcept { // move assignment
            std::swap(t, other.t);
            return *this;
        };
    };
};
