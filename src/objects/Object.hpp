#pragma once

#include "../webgpu/primitives/buffers/uniforms/ModelMatrixUniform.hpp"
#include "../webgpu/primitives/buffers/AttributeBuffer.hpp"
#include "../webgpu/primitives/buffers/IndexBuffer.hpp"
#include "../webgpu/Scene.hpp"
#include "ObjectResources.hpp"

namespace engine {

    class Object {
    protected:
        Context *context;
        Scene *scene;

        ObjectPipeline targetPipeline = ColoredTriangle;
    public:
        void setColor(glm::vec3 c);
        void setTexture(std::shared_ptr<engine::Texture2D::Texture> texture);
        ObjectPipeline currentTargetPipeline();

        bool SKIP_DRAW = false;

        std::shared_ptr<engine::ModelMatrixUniform> modelMatrix() const;
        std::shared_ptr<ObjectResources> resources = nullptr;
    };

}