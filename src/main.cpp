// [0, 1] instead of OpenGL [-1, 1] and l-h coord system
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_LEFT_HANDED
#include <glm/glm.hpp>

#include "engine.hpp"
#include "shape_helpers.hpp"
#include "webgpu/primitives/buffers/attributes/TexturedAttribute.hpp"
#include "webgpu/pipelines/IndexedTrianglePipeline.hpp"
#include "webgpu/pipelines/TrianglePipeline.hpp"
#include "webgpu/pipelines/TexturedTrianglePipeline.hpp"
#include "controllers/TurntableController.hpp"
#include "controllers/FreeviewController.hpp"

#include <iostream>

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

    auto modelMatrix = std::make_shared<engine::UniformModel>(c);

    auto uniforms = std::make_shared<engine::UniformViewProjection>(c, vec3(5.0, -5.0, 5.0),
                                                                    vec3(0, 0, 0));

    //auto controller = std::make_shared<FreeviewController>(uniforms);
    auto controller = std::make_shared<TurntableController>(uniforms);
    engine->setController(controller);

    bool success;

    /*
     * Shape pipeline
     */

    auto pyramidModelMatrix = std::make_shared<engine::UniformModel>(c);
    auto triangles = shape_helpers::makePyramid(vec3(0.0, 0.0, 1.0), 1.0, 1.0);
    auto data = shape_helpers::asIndexedTriangles(triangles);
    auto indexDataBuffer = std::make_shared<engine::IndexBuffer>(c, data.indexData);
    auto attributeDataBuffer = std::make_shared<engine::IndexedAttribute>(c, data);
    std::vector<IndexedTriangleObject> objects_indexed;
    objects_indexed.push_back({
            pyramidModelMatrix,
            indexDataBuffer,
            attributeDataBuffer,
    });
    auto itPipeline = std::make_shared<IndexedTrianglePipeline>(c, s, uniforms, objects_indexed);
    itPipeline->enableClear(Color{ 0.631, 0.666, 1.0, 1.0 });
    engine->addPipeline(itPipeline);

    /*
     * OBJs pipeline
     */

    auto mammothModelMatrix = std::make_shared<engine::UniformModel>(c);
    auto teapotModelMatrix = std::make_shared<engine::UniformModel>(c);
    std::vector<TriangleObject> tri_objects;
    {
        std::vector<TriangleVertexAttributes> vertexData;
        success = loadObjIntoTriangleData("resources/mammoth.obj", vertexData);
        //bool success = loadObjIntoTriangleData("resources/coffee_cup_obj.obj", vertexData);
        //bool success = loadObjIntoTriangleData("resources/bugatti.obj", vertexData);
        if (!success) {
            std::cerr << "Could not load geometry!" << std::endl;
            return 1;
        }

        auto attr = std::make_shared<engine::NonTexturedAttribute>(c, vertexData);

        tri_objects.push_back({mammothModelMatrix, attr});
    }
    {
        std::vector<TriangleVertexAttributes> vertexData;
        success = loadObjIntoTriangleData("resources/teapot.obj", vertexData);
        if (!success) {
            std::cerr << "Could not load geometry!" << std::endl;
            return 1;
        }

        auto attr = std::make_shared<engine::NonTexturedAttribute>(c, vertexData);

        tri_objects.push_back({teapotModelMatrix, attr});
    }
    engine->addPipeline(std::make_shared<TrianglePipeline>(c, s, uniforms, tri_objects));

    /*
     * Textured OBJ pipeline
     */
    // Texture test
    auto cubeModelMatrix = std::make_shared<engine::UniformModel>(c);
    //auto texture = std::make_shared<engine::DebugTexture2D>(engine, 256, 256);
    auto texture = std::make_shared<engine::Texture2D::common::BasicImgTexture>(
            engine->getContext().get(), "resources/img/grass.png");
    std::vector<UVTriangleVertexAttributes> vertexData3;
    success = loadTexturedObjIntoTriangleData("resources/cube.obj", vertexData3);
    if (!success) {
        std::cerr << "Could not load geometry!" << std::endl;
        return 1;
    }

    auto attrs = std::make_shared<engine::TexturedAttribute>(c, vertexData3);
    std::vector<TexturedTriangleObject> objects(1);
    objects[0] = { cubeModelMatrix, attrs };

    engine->addPipeline(std::make_shared<TexturedTrianglePipeline>(c, s, uniforms, texture, objects));

    /*
     * Plane pipeline
     */
    float z_pos = -0.5;
    float lg = 10;
    auto plane = shape_helpers::makeQuad(vec3(-lg, -lg, z_pos), vec3(lg, -lg, z_pos),
                                         vec3(lg, lg, z_pos), vec3(-lg, lg, z_pos));
    //auto planeTexture = std::make_shared<engine::ImageTexture2D>(engine, "resources/img/grass.png");
    auto planeTexture = std::make_shared<engine::Texture2D::common::DebugTexture>(c, 640, 640);
    auto planeData = shape_helpers::asTexturedTriangles(plane);
    auto planeBuffer = std::make_shared<engine::TexturedAttribute>(c, planeData);
    std::vector<TexturedTriangleObject> objects_2(1);
    objects_2[0] = { modelMatrix, planeBuffer };
    engine->addPipeline(std::make_shared<TexturedTrianglePipeline>(c, s, uniforms, planeTexture, objects_2));

    mammothModelMatrix->setTranslation(vec3(5.0, 0.0, 1.0));
    mammothModelMatrix->setScale(2.0);

    pyramidModelMatrix->setTranslation(vec3(-3.0, -3.0, -1.0));

    //int exit_result = engine->enterMainLoop();
    float rot = 0;
    while (!glfwWindowShouldClose(engine->getWindow())) {
        rot += 0.01;

        mammothModelMatrix->setRotation(rot);
        pyramidModelMatrix->setRotation(2*rot);

        glfwPollEvents();

        int result = engine->onFrame();
        if(result != 0)
            return result;
    }

    delete engine;

    //return exit_result;
    return 0;
}