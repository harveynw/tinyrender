#pragma once

#include "Object.hpp"

class TriangleImpl;
class QuadImpl;
class PyramidImpl;

namespace tinyrender {

    class Triangle : public Object {
    public:
        Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c);
        ~Triangle();
    };

    class Quad : public Object {
    public:
        Quad(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d);
        ~Quad();
    };

    class Pyramid : public Object {
    public:
        Pyramid(glm::vec3 centre, float height, float width);
        ~Pyramid();
    };

}