#ifdef __EMSCRIPTEN__
#include <emscripten/html5.h>
#endif
#include <FastNoiseLite.h>

#include <tinyrender/Engine.hpp>
#include <tinyrender/camera/FreeviewCamera.hpp>
#include <tinyrender/objects/Voxels.hpp>
#include <tinyrender/objects/voxel/ChunkGeometry.hpp>

/*
* tinyrender engine + voxel system
*/
std::unique_ptr<tinyrender::Engine> engine;
std::shared_ptr<tinyrender::Voxels> voxel;
std::shared_ptr<tinyrender::FreeviewCamera> camera;


/*
* Our procedural terrain generation algorithm
* Reference here: https://www.redblobgames.com/maps/terrain-from-noise/
*/
void 
terrain(std::array<char, N_VOXELS> &voxels, glm::ivec2 cornerXY) {
    // Populates a chunk with corner at _cornerXY_

    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

    std::array<std::array<int, SIZE_XY>, SIZE_XY> elevation;

    // Compute elevation
    for(int x = 0; x < SIZE_XY; x++) {
        for(int y = 0; y < SIZE_XY; y++) {
            // World voxel position from chunk position 
            int xx = x + cornerXY.x;
            int yy = y + cornerXY.y;

            elevation[x][y] = SIZE_Z * (1.0f + noise.GetNoise((float) xx, (float) yy))/2.0f;
        }
    }

    // Populate voxel data
    for(int i = 0; i < N_VOXELS; i++) {
        auto c = coord(i);

        if(c.z <= elevation[c.x][c.y])
            voxels[i] = (char) c.z;
        else
            voxels[i] = 0x00;
    }
}

int 
chebyshevDistance(ivec2 chunkA, ivec2 chunkB) {
    return std::max(abs(chunkA.x - chunkB.x), abs(chunkA.y - chunkB.y));
}

/*
* Our event loop, concerned with keeping chunks around the camera visible and generated.
*/
void onFrame() {
    int RENDER_DISTANCE = 3;

    // Chunk underneath camera
    glm::vec3 pos = camera->getPosition();
    ivec2 cameraChunk = ivec2(int(pos.x) / SIZE_XY, int(pos.y) / SIZE_XY);
 
    // Hide chunks beyond render distance
    for(auto &visibleChunk: voxel->visibleChunks()) {
        if(chebyshevDistance(cameraChunk, visibleChunk->chunkCoordinate()) > RENDER_DISTANCE)
            visibleChunk->setVisibility(tinyrender::CHUNK_HIDDEN);
    }

    // Ensure chunks are loaded surrounding camera
    for(int i = -RENDER_DISTANCE; i <= RENDER_DISTANCE; i++) {
        for(int j = -RENDER_DISTANCE; j <= RENDER_DISTANCE; j++) {
            auto selectedChunk = cameraChunk + ivec2(i, j);

            // Ensure initialised
            if(!voxel->chunkTracked(selectedChunk)) {
                auto v = voxel->getChunk(selectedChunk); 
                terrain(v->voxelData(), v->cornerCoordinate()); 
            }

            // Mark visible
            voxel->getChunk(selectedChunk)->setVisibility(tinyrender::CHUNK_VISIBLE);
        }
    }

    engine->onFrame();
}

/*
* Entry point
*/
int main (int, char**) {
    /*
     * Init engine
     */
    engine = std::make_unique<tinyrender::Engine>();
    engine->launch(640, 480);

    /*
     * Set helper class for moving camera about
     */
    camera = std::make_shared<tinyrender::FreeviewCamera>();
    engine->setCamera(camera);

    /*
     * Initialise voxel system
     */
    voxel = std::make_shared<tinyrender::Voxels>();
    engine->addObject(voxel);

    /*
     * Render loop
     */
    #ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(onFrame, 0, true);
    #else
    while (engine->isRunning())
        onFrame();
    #endif

    return 0;
}