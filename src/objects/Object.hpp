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
        virtual void update(float dt);
        virtual void setColor(glm::vec3 c);
        virtual void setTexture(std::shared_ptr<engine::Texture2D::Texture> texture);
        ObjectPipeline currentTargetPipeline();
        virtual ~Object() = default;

        bool SKIP_DRAW = false;

        std::shared_ptr<engine::ModelMatrixUniform> modelMatrix() const;
        std::shared_ptr<ObjectResources> resources = nullptr;
    };

}