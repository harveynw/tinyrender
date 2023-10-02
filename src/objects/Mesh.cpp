#include "loaders/obj.hpp"

#include "Mesh.hpp"


engine::Mesh::Mesh(Context *c, Scene *s, const std::string& path) {
    this->context = c;
    this->scene = s;

    // Populate
    if(STR_SUFFIX(path, std::string(".obj"))) {
        Obj obj;
        loadFromObjFile(path, obj);

        auto attrs = std::make_shared<engine::AttributeBuffer>(this->context, obj.data, obj.vertices);
        this->resources = std::make_shared<ObjectResources>(this->context, this->scene, attrs, ColoredTriangle);
    } else
        throw std::runtime_error("Mesh: unsupported file type");
}
