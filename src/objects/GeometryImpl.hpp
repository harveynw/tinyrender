#pragma once

#include <utility>
#include <string>
#include <glm/glm.hpp>

#include "objects/Geometry.hpp"
#include "loaders/Polygons.hpp"
#include "loaders/Shapes.hpp"
#include "ObjectImpl.hpp"
#include "ObjectResources.hpp"
#include "primitives/buffers/AttributeBuffer.hpp"

// The geometric objects don't have special logic for onRemove as their memory footprint is small.

class TriangleImpl : public ObjectImpl {
protected:
    glm::vec3 a, b, c;
public:
    TriangleImpl(glm::vec3 a, glm::vec3 b, glm::vec3 c);

    void onInit(Context *c, Scene *s) override;
};

class QuadImpl : public ObjectImpl {
protected:
    glm::vec3 a, b, c, d;
public:
    QuadImpl(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d);

    void onInit(Context *c, Scene *s) override;
};

class PyramidImpl : public ObjectImpl {
protected:
    glm::vec3 centre;
    float height, width;
public:
    PyramidImpl(glm::vec3 centre, float height, float width);
    
    void onInit(Context *c, Scene *s) override;
};
