#include "Chunk.hpp"

void 
Chunk::syncBuffer() {
    // Generate list of colored vertices by DFS
    auto start = glm::ivec3(0, 0, 0);
    Faces faces = visibleFrom(start, this->voxels);
    printf("%i faces found\n", (int) faces.size());

    // Generate attribute date
    Polygons p = {{}, 0};
    for(auto &face : faces) {
        auto midpoint = (glm::vec3(face.to) + glm::vec3(face.from)) * 0.5f;
        auto plane = glm::ivec3(1, 1, 1) - glm::abs(face.to - face.from);

        glm::vec3 a, b, c, d;

        if(plane.x && plane.y) {
            a = midpoint + glm::vec3(-0.5, -0.5, 0.0);
            b = midpoint + glm::vec3(-0.5, 0.5, 0.0);
            c = midpoint + glm::vec3(0.5, 0.5, 0.0);
            d = midpoint + glm::vec3(0.5, -0.5, 0.0);
        }
        if(plane.y && plane.z) {
            a = midpoint + glm::vec3(0.0, -0.5, -0.5);
            b = midpoint + glm::vec3(0.0, 0.5, -0.5);
            c = midpoint + glm::vec3(0.0, 0.5, 0.5);
            d = midpoint + glm::vec3(0.0, -0.5, 0.5);
        }
        if(plane.x && plane.z) {
            a = midpoint + glm::vec3(-0.5, 0.0, -0.5);
            b = midpoint + glm::vec3(0.5, 0.0, -0.5);
            c = midpoint + glm::vec3(0.5, 0.0, 0.5);
            d = midpoint + glm::vec3(-0.5, 0.0, 0.5);
        }

        Polygons p_new;
        loadQuad(p_new, a, b, c, d);
        p += p_new;
    }

    // Upload
    buffer = std::make_shared<engine::AttributeBuffer>(this->c, p.data, p.vertices);
    printf("Synced %i vertices uploading chunk data\n", (int) p.vertices);
}

Chunk::Chunk(Context *context): c(context)
{
    sphere(this->voxels, 6);
    syncBuffer();
}