#ifdef __EMSCRIPTEN__
#include <emscripten/html5.h>
#endif
#include <string>

#include <tinyrender/Engine.hpp>
#include <tinyrender/camera/TurntableCamera.hpp>
#include <tinyrender/objects/Mesh.hpp>
#include <tinyrender/objects/Cube.hpp>
#include <tinyrender/objects/WaveSim.hpp>

using glm::vec3;

int main (int, char**) {
    /*
     * Init engine
     */
    auto engine = std::make_unique<tinyrender::Engine>();
    engine->launch(640, 480);

    /*
     * Set helper class for moving camera about
     */

    auto camera = std::make_shared<tinyrender::TurntableCamera>();
    //auto camera = std::make_shared<tinyrender::FreeviewCamera>();
    engine->setCamera(camera);

    /*
     * Create the objects
     */
    {
        auto object = std::make_shared<tinyrender::Mesh>("resources/fourareen.obj");
        engine->addObject(object);
        object->setTexture("resources/fourareen2K_albedo.jpg");
    }
    {
        auto object = std::make_shared<tinyrender::Cube>();
        engine->addObject(object);

        object->setScale(0.2f);
        object->setTranslation(vec3(0,0,0.5));
        object->setColor(vec3(0.9, 0.9, 1.0));
    }
    {
        // Wave Sim
        auto object = std::make_shared<tinyrender::WaveSim>(50, 50);
        engine->addObject(object);

        object->setColor(vec3(0.0, 0.019, 0.301));
        object->setTranslation(vec3(0.0, 0.0, -0.5));
        object->setVisibility(tinyrender::OBJECT_VISIBLE);
    }

    /*
     * Render loop
     */
    #ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(
        [](void *userData) {
            tinyrender::Engine *app = reinterpret_cast<tinyrender::Engine*>(userData);
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