#include "loaders/Obj.hpp"

#include "Mesh.hpp"


tinyrender::Mesh::Mesh(const std::string& path): path(path) {}

void tinyrender::Mesh::onInit(Context *c, Scene *s) {
    Object::onInit(c, s);

    // Populate
    if(STR_SUFFIX(path, std::string(".obj"))) {
        Polygons p;
        loadFromObjFile(path, p);

        auto attrs = std::make_shared<tinyrender::AttributeBuffer>(this->context, p.data, p.vertices);
        this->resources = std::make_shared<ObjectResources>(this->context, this->scene, attrs, ColoredTriangle);
    } else
        throw std::runtime_error("Mesh: unsupported file type");
}
