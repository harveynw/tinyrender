
#ifdef __EMSCRIPTEN__
#include <emscripten/html5.h>
#endif

#include "tinyrender/Engine.hpp"
#include "tinyrender/camera/TurntableCamera.hpp"
#include "tinyrender/camera/FreeviewCamera.hpp"
#include "tinyrender/objects/Mesh.hpp"
#include "tinyrender/objects/Cube.hpp"
#include "tinyrender/objects/Geometry.hpp"
#include "tinyrender/objects/WaveSim.hpp"
#include "tinyrender/objects/Voxels.hpp"

using glm::vec3;

int main (int, char**) {
    /*
     * Init engine
     */

    auto engine = std::make_unique<Engine>();
    engine->launch(640, 480);

    /*
     * Set helper class for moving camera about
     */

    //auto camera = std::make_shared<TurntableCamera>(uniforms);
    auto camera = std::make_shared<FreeviewCamera>();
    engine->setCamera(camera);

    /*
     * Create the objects
     */
    glm::vec3 t = glm::vec3(60.0, -35.0, 35.0);
    auto boat_t = t + glm::vec3(0, 0, 0.5);
    {
        std::shared_ptr<tinyrender::Object> object = std::make_shared<tinyrender::Mesh>("resources/fourareen.obj");
        engine->addObject(object);

        object->setTranslation(boat_t);
        object->setTexture("resources/fourareen2K_albedo.jpg");
    }
    {
        auto object = std::make_shared<tinyrender::Cube>();
        engine->addObject(object);

        object->setScale(0.2f);
        object->setTranslation(vec3(0,0,0.5));
        object->setColor(vec3(0.9, 0.9, 1.0));
        engine->objects.push_back(object);
    }
    {
        auto object = std::make_shared<tinyrender::Pyramid>(vec3(0, 0, 3), 2, 3);
        engine->addObject(object);

        object->setScale(0.2f);
        object->setTranslation(vec3(0.0, 0.0, 5.0));
        object->setColor(vec3(1.0, 0.0, 1.0));
        engine->objects.push_back(object);
    }
    {
        // Wave Sim
        auto object = std::make_shared<tinyrender::WaveSim>(50, 50);
        engine->addObject(object);

        object->HIDDEN = false;
        object->setColor(vec3(0.0, 0.019, 0.301));

        object->setTranslation(t);
        object->setScale(1.2f);
    }
    {
        // Voxels
        auto voxels = std::make_shared<tinyrender::Voxels>();
        engine->addObject(voxels);
    }

    /*
     * Render loop
     */
    #ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(
        [](void *userData) {
            Engine *app = reinterpret_cast<Engine*>(userData);
            app->onFrame();
        },
        (void*) engine.get(),
        0, true
    );
    #else
    while (engine->isRunning())
        engine->onFrame();
    #endif

    return 0;
}