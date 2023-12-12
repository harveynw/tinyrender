#include "MeshImpl.hpp"

MeshImpl::MeshImpl(const std::string& path): path(path) {}

void MeshImpl::onInit(Context *c, Scene *s) {
    ObjectImpl::onInit(c, s);

    // Populate
    if(STR_SUFFIX(path, std::string(".obj"))) {
        Polygons p;
        loadFromObjFile(path, p);

        auto attrs = std::make_shared<AttributeBuffer>(this->context, p.data, p.vertices);
        this->resources = std::make_shared<ObjectResources>(this->context, this->scene, attrs, ColoredTriangle);
    } else
        throw std::runtime_error("Mesh: unsupported file type");
}

tinyrender::Mesh::Mesh(const std::string &path) {
    obj = std::make_unique<MeshImpl>(path);
}

tinyrender::Mesh::~Mesh() = default;
