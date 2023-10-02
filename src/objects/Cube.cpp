#include "Cube.hpp"

engine::Cube::Cube(Context *c, Scene *s) {
    this->context = c;
    this->scene = s;

    Polygons p;
    loadFromObj(defn, p);
    auto attrs = std::make_shared<engine::AttributeBuffer>(this->context, p.data, p.vertices);
    this->resources = std::make_shared<ObjectResources>(this->context, this->scene, attrs, ColoredTriangle);
}