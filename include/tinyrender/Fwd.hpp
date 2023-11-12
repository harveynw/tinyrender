// Forward declarations of internal classes implementated in src/

#pragma once

/*
* Tinyrender
*/
namespace tinyrender {
    class Camera;
    class Object;

    class AttributeBuffer;
    class ModelMatrixUniform;
    class ViewProjMatrixUniform;

    namespace Texture2D {
        class Texture;
        class WavesDataTexture;
    }
}
struct State;
struct Context;
struct Scene;

struct ObjectResources;

class TrianglePipeline;
class TexturedTrianglePipeline;
class WavesPipeline;

// Object specific
enum ObjectPipeline : short {
    TexturedTriangle,
    ColoredTriangle,
    Waves,
};
class ChunkMap;
class FFTfftw;
class NaiveWaveSim;

/*
* GLFW
*/
struct GLFWwindow;

/*
* wgpu
*/
namespace wgpu {
    class RenderPassEncoder;
}