#include "CubeImpl.hpp"

void 
CubeImpl::onInit(Context *c, Scene *s) {
    ObjectImpl::onInit(c, s);

    Polygons p;
    loadFromObj(defn, p);
    auto attrs = std::make_shared<AttributeBuffer>(this->context, p.data, p.vertices);
    this->resources = std::make_shared<ObjectResources>(this->context, this->scene, attrs, ColoredTriangle);
}

tinyrender::Cube::Cube() {
    obj = std::make_unique<CubeImpl>();
}

tinyrender::Cube::~Cube() = default;
