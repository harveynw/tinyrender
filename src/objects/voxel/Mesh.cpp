#include "Mesh.hpp"


bool 
Mesh::boundaryOcclusion(Face &face) {
    if(face.from.x < 0)
        return occlusionData[WEST][SIZE_XY * face.to.z + face.to.y];
    if(face.from.x == SIZE_XY)
        return occlusionData[EAST][SIZE_XY * face.to.z + face.to.y];
    if(face.from.y < 0)
        return occlusionData[SOUTH][SIZE_XY * face.to.z + face.to.x];
    if(face.from.y == SIZE_XY)
        return occlusionData[NORTH][SIZE_XY * face.to.z + face.to.x];
    return false;
}

void 
Mesh::updateOcculusionData(AdjacentMesh &adj) {
    auto update = [](VoxelSlice &slice, VoxelChunk &neighbourChunk, bool xzPlane, int plane) {
        // xzPlane defines whether scanning across X-Z or Y-Z plane defined by: 
        // x|y = plane in neighbourChunk
        for(int minor = 0; minor < SIZE_XY; minor++) {
            for(int z = 0; z < SIZE_Z; z++) {
                auto p = glm::ivec3(xzPlane ? minor : plane, xzPlane ? plane : minor, z);
                slice[SIZE_XY * z + minor] = !isTransparent(p, neighbourChunk);
            }
        }
    };

    if(std::shared_ptr<Mesh> ptr = adj.neighbours[NORTH].lock())
        update(occlusionData[NORTH], ptr->voxels, true, 0);
    
    if(std::shared_ptr<Mesh> ptr = adj.neighbours[SOUTH].lock())
        update(occlusionData[SOUTH], ptr->voxels, true, SIZE_XY-1);

    if(std::shared_ptr<Mesh> ptr = adj.neighbours[EAST].lock())
        update(occlusionData[EAST], ptr->voxels, false, 0);

    if(std::shared_ptr<Mesh> ptr = adj.neighbours[WEST].lock())
        update(occlusionData[WEST], ptr->voxels, false, SIZE_XY-1);
}

void 
Mesh::generateMesh() {
    // Generate list of colored vertices by DFS
    auto start = glm::ivec3(-1, -1, 0);
    Faces faces = visibleFrom(start, this->voxels);

    // Generate attribute date
    this->mesh = std::make_unique<Polygons>();
    for(auto &face : faces) {
        if(onBoundary(face.to) && boundaryOcclusion(face))
            continue;

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
        *(this->mesh.get()) += p_new;
    }
}

std::shared_ptr<tinyrender::AttributeBuffer> 
Mesh::gpu(Context *context) {
    return std::make_shared<tinyrender::AttributeBuffer>(context, this->mesh->data, this->mesh->vertices);
}
