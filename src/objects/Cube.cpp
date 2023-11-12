#include "objects/Cube.hpp"

#include "loaders/Obj.hpp"
#include "tiny_obj_loader.h"
#include "primitives/buffers/AttributeBuffer.hpp"
#include "ObjectResources.hpp"

void 
tinyrender::Cube::onInit(Context *c, Scene *s) {
    Object::onInit(c, s);

    Polygons p;
    loadFromObj(defn, p);
    auto attrs = std::make_shared<tinyrender::AttributeBuffer>(this->context, p.data, p.vertices);
    this->resources = std::make_shared<ObjectResources>(this->context, this->scene, attrs, ColoredTriangle);
}
