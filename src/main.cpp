// [0, 1] instead of OpenGL [-1, 1] and l-h coord system
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_LEFT_HANDED
#include <glm/glm.hpp>

#include "engine.hpp"
#include "shape_helpers.hpp"
#include "primitives/buffers/TexturedAttributeBuffer.hpp"
#include "pipelines/IndexedTrianglePipeline.hpp"
#include "pipelines/TrianglePipeline.hpp"
#include "pipelines/TexturedTrianglePipeline.hpp"
#include "primitives/textures/DepthTexture2D.hpp"
#include "primitives/textures/DebugTexture2D.hpp"
#include "primitives/textures/ImageTexture2D.hpp"
#include "controllers/TurntableController.hpp"
#include "controllers/FreeviewController.hpp"

#include <iostream>

using namespace wgpu;
using glm::mat4x4;
using glm::vec4;
using glm::vec3;
namespace fs = std::filesystem;

int main (int, char**) {
    // TODO: Implement dt into controllers
    // TODO: Implement vsynced event loop
    // TODO: Shift model matrix into own bind group tied to 'objects' not pipelines
    // TODO: MipMaps for Texture2D (+ ImageTextured2D)
    // TODO: More primitives

    auto *engine = new Engine(640, 480);
    engine->launch();

    auto modelMatrix = std::make_shared<engine::UniformModelBuffer>(engine);


    auto depthTexture = std::make_shared<engine::DepthTexture2D>(engine, 640, 480);
    auto uniforms = std::make_shared<engine::UniformViewProjectionBuffer>(engine, vec3(5.0, -5.0, 5.0),
                                                                          vec3(0, 0, 0));

    //auto controller = std::make_shared<FreeviewController>(uniforms);
    auto controller = std::make_shared<TurntableController>(uniforms);
    engine->setController(controller);

    bool success;

    /*
     * Shape pipeline, TODO: why has this started erorring yp here?
     */

    auto pyramidModelMatrix = std::make_shared<engine::UniformModelBuffer>(engine);
    auto triangles = shape_helpers::makePyramid(vec3(0.0, 0.0, 1.0), 1.0, 1.0);
    auto data = shape_helpers::asIndexedTriangles(triangles);
    auto indexDataBuffer = std::make_shared<engine::IndexBuffer>(engine, data.indexData);
    auto attributeDataBuffer = std::make_shared<engine::IndexedAttributeBuffer>(engine, data);
    std::vector<IndexedTriangleObject> objects_indexed;
    objects_indexed.push_back({
            pyramidModelMatrix,
            indexDataBuffer,
            attributeDataBuffer,
    });
    auto itPipeline = std::make_shared<IndexedTrianglePipeline>(engine, uniforms, depthTexture, objects_indexed);
    itPipeline->enableClear(Color{ 0.631, 0.666, 1.0, 1.0 });
    engine->addPipeline(itPipeline);

    /*
     * OBJs pipeline
     */

    auto mammothModelMatrix = std::make_shared<engine::UniformModelBuffer>(engine);
    auto teapotModelMatrix = std::make_shared<engine::UniformModelBuffer>(engine);
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

        auto attr = std::make_shared<engine::NonTexturedAttributeBuffer>(engine, vertexData);

        tri_objects.push_back({mammothModelMatrix, attr});
    }
    {
        std::vector<TriangleVertexAttributes> vertexData;
        success = loadObjIntoTriangleData("resources/teapot.obj", vertexData);
        if (!success) {
            std::cerr << "Could not load geometry!" << std::endl;
            return 1;
        }

        auto attr = std::make_shared<engine::NonTexturedAttributeBuffer>(engine, vertexData);

        tri_objects.push_back({teapotModelMatrix, attr});
    }
    engine->addPipeline(std::make_shared<TrianglePipeline>(engine, uniforms, depthTexture, tri_objects));

    /*
     * Textured OBJ pipeline
     */
    // Texture test
    auto cubeModelMatrix = std::make_shared<engine::UniformModelBuffer>(engine);
    //auto texture = std::make_shared<engine::DebugTexture2D>(engine, 256, 256);
    auto texture = std::make_shared<engine::ImageTexture2D>(engine, "resources/img/grass.png");
    std::vector<UVTriangleVertexAttributes> vertexData3;
    success = loadTexturedObjIntoTriangleData("resources/cube.obj", vertexData3);
    if (!success) {
        std::cerr << "Could not load geometry!" << std::endl;
        return 1;
    }

    auto attrs = std::make_shared<engine::TexturedAttributeBuffer>(engine, vertexData3);
    std::vector<TexturedTriangleObject> objects(1);
    objects[0] = { cubeModelMatrix, attrs };

    engine->addPipeline(std::make_shared<TexturedTrianglePipeline>(engine, uniforms, texture, depthTexture, objects));

    /*
     * Plane pipeline
     */
    float z_pos = -0.5;
    float lg = 10;
    auto plane = shape_helpers::makeQuad(vec3(-lg, -lg, z_pos), vec3(lg, -lg, z_pos),
                                         vec3(lg, lg, z_pos), vec3(-lg, lg, z_pos));
    auto planeTexture = std::make_shared<engine::ImageTexture2D>(engine, "resources/img/grass.png");
    auto planeData = shape_helpers::asTexturedTriangles(plane);
    auto planeBuffer = std::make_shared<engine::TexturedAttributeBuffer>(engine, planeData);
    std::vector<TexturedTriangleObject> objects_2(1);
    objects_2[0] = { modelMatrix, planeBuffer };
    engine->addPipeline(std::make_shared<TexturedTrianglePipeline>(engine, uniforms, planeTexture,
                                                                   depthTexture, objects_2));

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