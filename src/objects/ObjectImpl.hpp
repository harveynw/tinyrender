#pragma once

#include <utility>
#include <memory>
#include <glm/glm.hpp>

#include "../webgpu/Context.hpp"
#include "../webgpu/Scene.hpp"
#include "../webgpu/primitives/uniforms/ModelMatrixUniform.hpp"
#include "../webgpu/primitives/buffers/AttributeBuffer.hpp"
#include "../webgpu/primitives/buffers/IndexBuffer.hpp"
#include "../webgpu/Scene.hpp"
#include "../webgpu/primitives/textures/Texture2D.hpp"
#include "../State.hpp"
#include "ObjectResources.hpp"
#include "objects/Object.hpp"


class ObjectImpl {
protected:
    Context *context;
    Scene *scene;

    bool isInitialised = false;
public:
    ObjectImpl() = default;
    
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

    virtual ~ObjectImpl() = default;

    virtual ObjectPipeline currentTargetPipeline();

    bool HIDDEN = false;

    virtual std::shared_ptr<ModelMatrixUniform> modelMatrix() const;
    std::shared_ptr<ObjectResources> resources = nullptr;
};
