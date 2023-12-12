#include "GeometryImpl.hpp"

TriangleImpl::TriangleImpl(glm::vec3 a, glm::vec3 b, glm::vec3 c): a(a), b(b), c(c) {}

void 
TriangleImpl::onInit(Context *c, Scene *s) {
    ObjectImpl::onInit(c, s);

    Polygons p;
    loadTriangle(p, this->a, this->b, this->c);
    auto attrs = std::make_shared<AttributeBuffer>(this->context, p.data, p.vertices);
    this->resources = std::make_shared<ObjectResources>(this->context, this->scene, attrs, ColoredTriangle);
}

QuadImpl::QuadImpl(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d): a(a), b(b), c(c), d(d) {}

void 
QuadImpl::onInit(Context *c, Scene *s) {
    ObjectImpl::onInit(c, s);

    Polygons p;
    loadQuad(p, this->a, this->b, this->c, this->d);
    auto attrs = std::make_shared<AttributeBuffer>(this->context, p.data, p.vertices);
    this->resources = std::make_shared<ObjectResources>(this->context, this->scene, attrs, ColoredTriangle);
}

PyramidImpl::PyramidImpl(glm::vec3 centre, float height, float width): centre(centre), height(height), width(width) {}

void 
PyramidImpl::onInit(Context *c, Scene *s) {
    ObjectImpl::onInit(c, s);

    Polygons p;
    loadPyramid(p, centre, height, width);

    auto attrs = std::make_shared<AttributeBuffer>(this->context, p.data, p.vertices);
    this->resources = std::make_shared<ObjectResources>(this->context, this->scene, attrs, ColoredTriangle);
}

tinyrender::Triangle::Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
    obj = std::make_unique<TriangleImpl>(a, b, c);
}

tinyrender::Triangle::~Triangle() = default;

tinyrender::Quad::Quad(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d) {
    obj = std::make_unique<QuadImpl>(a, b, c, d);
}

tinyrender::Quad::~Quad() = default;

tinyrender::Pyramid::Pyramid(glm::vec3 centre, float height, float width) {
    obj = std::make_unique<PyramidImpl>(centre, height, width);
}

tinyrender::Pyramid::~Pyramid() = default;
