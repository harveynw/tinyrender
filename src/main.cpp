
#ifdef __EMSCRIPTEN__
#include <emscripten/html5.h>
#endif

#include "engine.hpp"
#include "camera/TurntableCamera.hpp"
#include "camera/FreeviewCamera.hpp"
#include "objects/Mesh.hpp"
#include "objects/Cube.hpp"
#include "objects/Geometry.hpp"
#include "objects/WaveSim.hpp"
#include "objects/Voxels.hpp"

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
        auto texture = std::make_shared<tinyrender::Texture2D::common::BasicImgRepeatingTexture>(
                engine->getContext().get(), "resources/fourareen2K_albedo.jpg");
        std::shared_ptr<tinyrender::Object> object = std::make_shared<tinyrender::Mesh>("resources/fourareen.obj");
        engine->addObject(object);

        object->modelMatrix()->setTranslation(boat_t);
        object->setTexture(texture);
    }
    {
        auto object = std::make_shared<tinyrender::Cube>();
        engine->addObject(object);

        object->modelMatrix()->setScale(0.2f);
        object->modelMatrix()->setTranslation(vec3(0,0,0.5));
        object->setColor(vec3(0.9, 0.9, 1.0));
        engine->objects.push_back(object);
    }
    {
        auto object = std::make_shared<tinyrender::Pyramid>(vec3(0, 0, 3), 2, 3);
        engine->addObject(object);

        object->modelMatrix()->setScale(0.2f);
        object->modelMatrix()->setTranslation(vec3(0.0, 0.0, 5.0));
        object->setColor(vec3(1.0, 0.0, 1.0));
        engine->objects.push_back(object);
    }
    {
        // Wave Sim
        auto object = std::make_shared<tinyrender::WaveSim>(50, 50);
        engine->addObject(object);

        object->HIDDEN = false;
        object->setColor(vec3(0.0, 0.019, 0.301));

        object->modelMatrix()->setTranslation(t);
        object->modelMatrix()->setScale(1.2f);
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