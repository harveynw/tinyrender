#include "Geometry.hpp"


engine::Triangle::Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c): a(a), b(b), c(c) {}

void 
engine::Triangle::onInit(Context *c, Scene *s) {
    Object::onInit(c, s);

    Polygons p;
    loadTriangle(p, this->a, this->b, this->c);
    auto attrs = std::make_shared<engine::AttributeBuffer>(this->context, p.data, p.vertices);
    this->resources = std::make_shared<ObjectResources>(this->context, this->scene, attrs, ColoredTriangle);
}

engine::Quad::Quad(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d): a(a), b(b), c(c), d(d) {}

void 
engine::Quad::onInit(Context *c, Scene *s) {
    Object::onInit(c, s);

    Polygons p;
    loadQuad(p, this->a, this->b, this->c, this->d);
    auto attrs = std::make_shared<engine::AttributeBuffer>(this->context, p.data, p.vertices);
    this->resources = std::make_shared<ObjectResources>(this->context, this->scene, attrs, ColoredTriangle);
}

engine::Pyramid::Pyramid(glm::vec3 centre, float height, float width): centre(centre), height(height), width(width) {}

void 
engine::Pyramid::onInit(Context *c, Scene *s) {
    Object::onInit(c, s);

    Polygons p;
    loadPyramid(p, centre, height, width);

    auto attrs = std::make_shared<engine::AttributeBuffer>(this->context, p.data, p.vertices);
    this->resources = std::make_shared<ObjectResources>(this->context, this->scene, attrs, ColoredTriangle);
}
