#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_LEFT_HANDED
#include <glm/glm.hpp>

#ifdef __EMSCRIPTEN__
#include <emscripten/html5.h>
#endif

#include "engine.hpp"
#include "controllers/TurntableController.hpp"
#include "controllers/FreeviewController.hpp"
#include "objects/Mesh.hpp"
#include "objects/Cube.hpp"
#include "objects/Geometry.hpp"
#include "objects/WaveSim.hpp"

using namespace wgpu;
using glm::mat4x4;
using glm::vec4;
using glm::vec3;


int main (int, char**) {
    /*
     * Init engine
     */

    auto *engine = new Engine(640, 480);
    engine->launch();
    Context *c = engine->getContext().get();
    Scene *s = engine->getScene().get();

    /*
     * Set helper class for moving camera about
     */

    //auto controller = std::make_shared<TurntableController>(uniforms);
    auto controller = std::make_shared<FreeviewController>();
    engine->setController(controller);

    /*
     * Create the objects
     */
    {
        auto texture = std::make_shared<engine::Texture2D::common::BasicImgRepeatingTexture>(
                engine->getContext().get(), "resources/fourareen2K_albedo.jpg");
        std::shared_ptr<engine::Object> object = std::make_shared<engine::Mesh>(c, s, "resources/fourareen.obj");
        engine->objects.push_back(object);
        object->modelMatrix()->setTranslation(vec3(0,0,0.5));
        object->setTexture(texture);
    }
    {
        auto object = std::make_shared<engine::Cube>(c, s);
        object->modelMatrix()->setScale(0.2f);
        object->modelMatrix()->setTranslation(vec3(0,0,0.5));
        object->setColor(vec3(0.9, 0.9, 1.0));
        engine->objects.push_back(object);
    }
    {
        auto object = std::make_shared<engine::Pyramid>(c, s, vec3(0, 0, 3), 2, 3);
        object->SKIP_DRAW = true;
        object->modelMatrix()->setScale(0.2f);
        object->setColor(vec3(1.0, 0.0, 1.0));
        engine->objects.push_back(object);
    }

    // Wave Sim
    auto waveSim = std::make_shared<engine::WaveSim>(c, s, 50, 50);
    waveSim->SKIP_DRAW = false;
    waveSim->setColor(vec3(0.0, 0.019, 0.301));
    engine->objects.push_back(waveSim);

    /*
     * Render loop
     */
    #ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(
        [](void *userData) {
            Engine& app = *reinterpret_cast<Engine*>(userData);
            app.onFrame();
        },
        (void*)engine,
        0, true
    );
    #else
    while (engine->isRunning())
        engine->onFrame();
    #endif

    delete engine;
    return 0;
}