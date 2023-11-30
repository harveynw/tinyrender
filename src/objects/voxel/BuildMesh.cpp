#include "BuildMesh.hpp"
#include "Chunk.hpp"

namespace {
    struct Face {
        ivec3 from; // May be from outside chunk
        ivec3 to; // Always inside chunk
        char voxel; // Voxel value that face is on
    };

    std::shared_ptr<VoxelMesh> facesToPolygons(std::vector<Face> &faces) {
        auto mesh = std::make_shared<VoxelMesh>();
        mesh->reserve(4 * faces.size());

        for(auto &f : faces) {
            auto midpoint = (vec3(f.to) + vec3(f.from)) * 0.5f;
            auto plane = ivec3(1, 1, 1) - glm::abs(f.to - f.from);

            // Find corners
            vec3 a, b, c, d;
            if(plane.x && plane.y) {
                a = midpoint + vec3(-0.5, -0.5, 0.0);
                b = midpoint + vec3(-0.5, 0.5, 0.0);
                c = midpoint + vec3(0.5, 0.5, 0.0);
                d = midpoint + vec3(0.5, -0.5, 0.0);
            }
            if(plane.y && plane.z) {
                a = midpoint + vec3(0.0, -0.5, -0.5);
                b = midpoint + vec3(0.0, 0.5, -0.5);
                c = midpoint + vec3(0.0, 0.5, 0.5);
                d = midpoint + vec3(0.0, -0.5, 0.5);
            }
            if(plane.x && plane.z) {
                a = midpoint + vec3(-0.5, 0.0, -0.5);
                b = midpoint + vec3(0.5, 0.0, -0.5);
                c = midpoint + vec3(0.5, 0.0, 0.5);
                d = midpoint + vec3(-0.5, 0.0, 0.5);
            }

            // Generate attributes
            mesh->push_back({a, 0.0f});
            mesh->push_back({b, 0.0f});
            mesh->push_back({c, 0.0f});
            mesh->push_back({d, 0.0f});
        }

        return mesh;
    }

    vector<Face>
    DFS_visibility(ivec3 &c0, array<char, N_VOXELS> &voxels) {
        vector<Face> f;

        // (iterative)-DFS to populate f
        std::unordered_map<int, bool> visited;
        std::stack<ivec3> s;
        s.push(c0);
        while(!s.empty()) {
            auto c = s.top();
            int c_hash = hash(c);
            s.pop();

            if(visited.count(c_hash) > 0)
                continue;
            visited[c_hash] = true;

            for(auto &c_dash : adjacentCoordinates(c)) {
                if(visited.count(hash(c_dash)) > 0)
                    continue;

                if(!inBounds(c_dash) || voxels[idx(c_dash)] == 0x00) {
                    s.push(c_dash); // Air to search from
                } else {
                    // Face found
                    f.push_back({c, c_dash, voxels[idx(c_dash)]}); 
                }
            }
        }

        return f;
    }

    bool boundaryOcclusion(Face face, NeighbourData &neighbourData) {
        if(face.from.x < 0)
            return neighbourData.west[SIZE_XY * face.to.z + face.to.y];
        if(face.from.x == SIZE_XY)
            return neighbourData.east[SIZE_XY * face.to.z + face.to.y];
        if(face.from.y < 0)
            return neighbourData.south[SIZE_XY * face.to.z + face.to.x];
        if(face.from.y == SIZE_XY)
            return neighbourData.north[SIZE_XY * face.to.z + face.to.x];
        return false;
    }
}

NeighbourData extractBoundaries(Chunks &chunks, ivec2 chunkCoordinate)
{
    NeighbourData data{};

    auto update = [](array<bool, SIZE_XY * SIZE_Z> &slice, array<char, N_VOXELS> &neighbourChunk, bool xzPlane, int plane) {
        // xzPlane defines whether scanning across X-Z or Y-Z plane defined by: 
        // x|y = plane in neighbourChunk
        for(int minor = 0; minor < SIZE_XY; minor++) {
            for(int z = 0; z < SIZE_Z; z++) {
                auto p = ivec3(xzPlane ? minor : plane, xzPlane ? plane : minor, z);
                slice[SIZE_XY * z + minor] = !isTransparent(p, neighbourChunk);
            }
        }
    };
    auto n = chunkCoordinate + ivec2(0, 1);
    auto s = chunkCoordinate + ivec2(0, -1);
    auto e = chunkCoordinate + ivec2(1, 0);
    auto w = chunkCoordinate + ivec2(-1, 0);

    if(chunks.chunkDisplayed(n) )
        update(data.north, chunks.getChunk(n)->voxels, true, 0);
    if(chunks.chunkDisplayed(s))
        update(data.south, chunks.getChunk(s)->voxels, true, SIZE_XY-1);
    if(chunks.chunkDisplayed(e) )
        update(data.east, chunks.getChunk(e)->voxels, false, 0);
    if(chunks.chunkDisplayed(w))
        update(data.west, chunks.getChunk(w)->voxels, false, SIZE_XY-1);

    return data;
}

std::shared_ptr<VoxelMesh> buildMeshCullBoundaries(array<char, N_VOXELS> chunk, NeighbourData neighbourData)
{
    // Generate list of vertices by DFS
    auto start = ivec3(-1, -1, 0);
    auto faces = DFS_visibility(start, chunk);

    // Generate attribute date
    vector<Face> pruned;
    for(auto &face : faces) {
        if(onBoundary(face.to) && boundaryOcclusion(face, neighbourData))
            continue;
        pruned.push_back(face);
    }

    return facesToPolygons(pruned);
}

std::shared_ptr<VoxelMesh> buildMeshNaive(array<char, N_VOXELS> chunk)
{
    // Generate list of vertices by DFS
    auto start = ivec3(-1, -1, 0);
    auto faces = DFS_visibility(start, chunk);

    return facesToPolygons(faces);
}

std::shared_ptr<VoxelMesh> buildMeshGridSearch(array<char, N_VOXELS> chunk)
{
    vector<Face> faces;
    for(int x = -1; x <= SIZE_XY; x++) {
        for(int y = -1; y <= SIZE_XY; y++) {
            for(int z = -1; z <= SIZE_Z; z++) {
                auto p = ivec3(x, y, z);
                if(inBounds(p) && !isTransparent(p, chunk))
                    continue;
                
                std::vector<glm::ivec3> directions = 
                {
                    p + ivec3(1, 0, 0),
                    p + ivec3(-1, 0, 0),
                    p + ivec3(0, 1, 0),
                    p + ivec3(0, -1, 0),
                    p + ivec3(0, 0, 1),
                    p + ivec3(0, 0, -1)
                };

                for(auto &d : directions) {
                    if(inBounds(d) && !isTransparent(d, chunk))
                        faces.push_back({p, d, chunk[idx(d)]});
                }
            }
        }
    }
    return facesToPolygons(faces);
}
