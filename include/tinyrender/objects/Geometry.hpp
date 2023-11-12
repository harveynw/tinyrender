#pragma once

#include <utility>
#include <string>
#include <glm/glm.hpp>

#include "Object.hpp"

// The geometric objects don't have special logic for onRemove as their memory footprint is small.

namespace tinyrender {

    class Triangle : public Object {
    protected:
        glm::vec3 a, b, c;
    public:
        Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c);

        void onInit(Context *c, Scene *s) override;
    };

    class Quad : public Object {
    protected:
        glm::vec3 a, b, c, d;
    public:
        Quad(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d);

        void onInit(Context *c, Scene *s) override;
    };

    class Pyramid : public Object {
    protected:
        glm::vec3 centre;
        float height, width;
    public:
        Pyramid(glm::vec3 centre, float height, float width);
        
        void onInit(Context *c, Scene *s) override;
    };
}