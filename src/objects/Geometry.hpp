#pragma once

#include "tiny_obj_loader.h"
#include <utility>
#include <string>
#include <glm/glm.hpp>

#include "Object.hpp"
#include "loaders/Polygons.hpp"
#include "loaders/Shapes.hpp"


namespace engine {

    class Triangle : public Object {
    public:
        Triangle(Context *context, Scene *scene, glm::vec3 a, glm::vec3 b, glm::vec3 c);
    };

    class Quad : public Object {
    public:
        Quad(Context *context, Scene *scene, glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d);
    };

    class Pyramid : public Object {
    public:
        Pyramid(Context *context, Scene *scene, glm::vec3 centre, float height, float width);
    };
}