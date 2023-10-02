#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_LEFT_HANDED
#include <glm/glm.hpp>

#include "engine.hpp"
#include "controllers/TurntableController.hpp"
#include "controllers/FreeviewController.hpp"
#include "objects/Mesh.hpp"
#include "objects/Cube.hpp"

using namespace wgpu;
using glm::mat4x4;
using glm::vec4;
using glm::vec3;


int main (int, char**) {
    // TODO: Implement dt into controllers
    // TODO: Shift model matrix into own bind group tied to 'objects' not pipelines
    // TODO: More primitives

    auto *engine = new Engine(640, 480);
    engine->launch();

    Context *c = engine->getContext().get();
    Scene *s = engine->getScene().get();

    //auto controller = std::make_shared<TurntableController>(uniforms);
    auto controller = std::make_shared<FreeviewController>();
    engine->setController(controller);

    // Texture test
    auto texture = std::make_shared<engine::Texture2D::common::BasicImgRepeatingTexture>(
            engine->getContext().get(), "resources/fourareen2K_albedo.jpg");
    std::shared_ptr<engine::Object> object = std::make_shared<engine::Mesh>(c, s, "resources/fourareen.obj");
    engine->objects.push_back(object);
    object->setTexture(texture);
    //object->setColor(vec3(1.0, 0.5, 0.2));

    auto object2 = std::make_shared<engine::Cube>(c, s);
    object2->modelMatrix()->setScale(0.2f);
    object2->setColor(vec3(0.9, 0.9, 1.0));
    engine->objects.push_back(object2);

    while (!glfwWindowShouldClose(engine->getWindow())) {
        glfwPollEvents();

        int result = engine->onFrame();
        if(result != 0)
            return result;
    }

    delete engine;
    return 0;
}