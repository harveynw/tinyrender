#include "Geometry.hpp"


engine::Triangle::Triangle(Context *context, Scene *scene, glm::vec3 a, glm::vec3 b, glm::vec3 c) {
    this->context = context;
    this->scene = scene;

    Polygons p;
    loadTriangle(p, a, b, c);

    auto attrs = std::make_shared<engine::AttributeBuffer>(this->context, p.data, p.vertices);
    this->resources = std::make_shared<ObjectResources>(this->context, this->scene, attrs, ColoredTriangle);
}

engine::Quad::Quad(Context *context, Scene *scene, glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d) {
    this->context = context;
    this->scene = scene;

    Polygons p;
    loadQuad(p, a, b, c, d);

    auto attrs = std::make_shared<engine::AttributeBuffer>(this->context, p.data, p.vertices);
    this->resources = std::make_shared<ObjectResources>(this->context, this->scene, attrs, ColoredTriangle);
}

engine::Pyramid::Pyramid(Context *context, Scene *scene, glm::vec3 centre, float height, float width) {
    this->context = context;
    this->scene = scene;

    Polygons p;
    loadPyramid(p, centre, height, width);

    auto attrs = std::make_shared<engine::AttributeBuffer>(this->context, p.data, p.vertices);
    this->resources = std::make_shared<ObjectResources>(this->context, this->scene, attrs, ColoredTriangle);
}
