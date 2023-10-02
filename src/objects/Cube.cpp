#include "Cube.hpp"

engine::Cube::Cube(Context *c, Scene *s) {
    this->context = c;
    this->scene = s;

    Obj obj;
    loadFromObj(defn, obj);
    auto attrs = std::make_shared<engine::AttributeBuffer>(this->context, obj.data, obj.vertices);
    this->resources = std::make_shared<ObjectResources>(this->context, this->scene, attrs, ColoredTriangle);
}