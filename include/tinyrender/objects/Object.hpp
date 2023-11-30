#pragma once

#include <utility>
#include <memory>
#include <glm/glm.hpp>

#include "../Fwd.hpp"
#include "../Transform.hpp"

namespace tinyrender {

    class Object {
    protected:
        Context *context;
        Scene *scene;

        bool isInitialised = false;
    public:
        virtual void onInit(Context *c, Scene *s);
        virtual void onUpdate(State &state, float dt);
        virtual void onDraw(wgpu::RenderPassEncoder &renderPass, int vertexBufferSlot, int bindGroupSlot);
        virtual void onRemove() ;

        virtual void setColor(glm::vec3 c);
        virtual void setTexture(std::string path);

        void setScale(float s); 
        void setTranslation(glm::vec3 t);
        void setRotationX(float r); 
        void setRotationY(float r); 
        void setRotationZ(float r);

        virtual ~Object() = default;

        virtual ObjectPipeline currentTargetPipeline();

        bool HIDDEN = false;

        virtual std::shared_ptr<tinyrender::ModelMatrixUniform> modelMatrix() const;
        std::shared_ptr<ObjectResources> resources = nullptr;
    };

}