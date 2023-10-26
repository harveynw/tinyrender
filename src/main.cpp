
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
#include "objects/Voxels.hpp"

using glm::mat4x4;
using glm::vec4;
using glm::vec3;


int main (int, char**) {
    /*
     * Init engine
     */

    auto *engine = new Engine(640, 480);
    engine->launch();

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
        std::shared_ptr<engine::Object> object = std::make_shared<engine::Mesh>("resources/fourareen.obj");
        engine->addObject(object);

        object->modelMatrix()->setTranslation(vec3(0,0,0.5));
        object->setTexture(texture);
    }
    {
        auto object = std::make_shared<engine::Cube>();
        engine->addObject(object);

        object->modelMatrix()->setScale(0.2f);
        object->modelMatrix()->setTranslation(vec3(0,0,0.5));
        object->setColor(vec3(0.9, 0.9, 1.0));
        engine->objects.push_back(object);
    }
    {
        auto object = std::make_shared<engine::Pyramid>(vec3(0, 0, 3), 2, 3);
        engine->addObject(object);

        object->modelMatrix()->setScale(0.2f);
        object->modelMatrix()->setTranslation(vec3(0.0, 0.0, 5.0));
        object->setColor(vec3(1.0, 0.0, 1.0));
        engine->objects.push_back(object);
    }
    {
        // Wave Sim
        auto object = std::make_shared<engine::WaveSim>(50, 50);
        engine->addObject(object);

        object->SKIP_DRAW = false;
        object->setColor(vec3(0.0, 0.019, 0.301));
    }
    {
        // Voxels
        auto voxels = std::make_shared<engine::Voxels>();
        engine->addObject(voxels);

        voxels->modelMatrix()->setScale(1.0f/2.0f);
    }

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