#pragma once

#include <utility>
#include <string>

#include "ObjectImpl.hpp"
#include "loaders/Obj.hpp"
#include "tiny_obj_loader.h"
#include "ObjectResources.hpp"
#include "primitives/buffers/AttributeBuffer.hpp"
#include "objects/Mesh.hpp"

#define STR_SUFFIX(s, suffix) s.substr(s.size()-suffix.size(), suffix.size()) == suffix


class MeshImpl : public ObjectImpl {
protected:
    std::string path;
public:
    MeshImpl(const std::string& path);

    void onInit(Context *c, Scene *s) override;
};