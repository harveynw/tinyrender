#include "BuildMesh.hpp"

#include <cstring>
#include <stack>

#include "objects/voxel/Chunk.hpp"
#include "ChunkImpl.hpp"


namespace {
    struct Face {
        ivec3 from; // May be from outside chunk
        ivec3 to; // Always inside chunk
        char voxel; // Voxel value that face is on

        // Ambient Occlusion Values
        array<char, 4> ao = { 0x00, 0x00, 0x00, 0x00 }; 
    };

    char voxelLookup(ivec3 coord, array<char, N_VOXELS> &voxels, NeighbourData &neighbourData) {
        // Gives you the voxel value at coord, including for 1 voxel width layer around the chunk

        // Easiest case, inside chunk
        if(inBounds(coord))
            return voxels[idx(coord)];
        
        // Above and below chunks treated as air
        if(coord.z < 0 || coord.z > SIZE_Z-1)
            return 0x00;

        // Check corners
        if(coord.x < 0 && coord.y < 0)
            return neighbourData.cornerSW[coord.z];
        if(coord.x < 0 && coord.y == SIZE_XY)
            return neighbourData.cornerNW[coord.z];
        if(coord.x == SIZE_XY && coord.y == SIZE_XY)
            return neighbourData.cornerNE[coord.z];
        if(coord.x == SIZE_XY && coord.y < 0)
            return neighbourData.cornerSE[coord.z];
        
        // Check side walls
        if(coord.x < 0)
            return neighbourData.west[SIZE_XY * coord.z + coord.y];
        if(coord.x == SIZE_XY)
            return neighbourData.east[SIZE_XY * coord.z + coord.y];
        if(coord.y < 0)
            return neighbourData.south[SIZE_XY * coord.z + coord.x];
        if(coord.y == SIZE_XY)
            return neighbourData.north[SIZE_XY * coord.z + coord.x];
        
        throw std::runtime_error("Coordinate invalid");
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

    void populateAOData(array<char, N_VOXELS> &voxels, vector<Face> &faces, NeighbourData &neighbourData) {
        // Ambient Occlusion Algorithm. Populates Face.ao for each element in faces
        // Source: https://0fps.net/2013/07/03/ambient-occlusion-for-minecraft-like-worlds/
        auto calculateAO = [](char &ao, bool side1, bool side2, bool corner) {
            if(side1 && side2)
                ao = 0x00;
            else
                ao = 3 - (side1 + side2 + corner);
        };

        // Convenience capture 
        auto voxel = [&voxels, &neighbourData](ivec3 coord) { return voxelLookup(coord, voxels, neighbourData); };

        for(auto &f: faces) {
            // Compute subspace parallel to face
            ivec3 direction = f.to - f.from;
            ivec3 plane = ivec3(1, 1, 1) - glm::abs(direction);

            // Subspace decomposed into vectors
            auto ax1 = ivec3(0, plane.x && plane.y, plane.z);
            auto ax2 = ivec3(plane.x, plane.y && plane.z, 0);

            // We rely on implicit char -> bool for the voxel value
            calculateAO(f.ao[0], voxel(f.from - ax1), voxel(f.from - ax2), voxel(f.from - ax1 - ax2));
            calculateAO(f.ao[1], voxel(f.from - ax2), voxel(f.from + ax1), voxel(f.from - ax2 + ax1));
            calculateAO(f.ao[2], voxel(f.from + ax1), voxel(f.from + ax2), voxel(f.from + ax1 + ax2));
            calculateAO(f.ao[3], voxel(f.from + ax2), voxel(f.from - ax1), voxel(f.from + ax2 - ax1));
        }
    }

    VoxelVertexAttribute buildAttribute(vec3 position, char material, char ao, char normal) {
        // Packs information into a VoxelVertexAttribute data structure
        std::array<char, 4> data;
        data[0] = material;
        data[1] = ao;
        data[2] = normal;
        data[3] = 0x00;

        VoxelVertexAttribute attr;
        attr.position = position;
        memcpy(&attr.data, data.begin(), 4);

        return attr;
    }

    std::shared_ptr<VoxelMesh> facesToPolygons(std::vector<Face> &faces) {
        // Builds a mesh, a collection of VoxelVertexAttribute's, from multiple faces
        auto mesh = std::make_shared<VoxelMesh>();
        mesh->reserve(6 * faces.size());

        for(auto &f : faces) {
            auto midpoint = (vec3(f.to) + vec3(f.from)) * 0.5f;
            auto plane = ivec3(1, 1, 1) - glm::abs(f.to - f.from);

            // Find corners
            // B ---- C
            //  |    |
            //  |    |
            // A ---- D
            vec3 a, b, c, d;
            if(plane.x && plane.y) {
                a = midpoint + vec3(-0.5, -0.5, 0.0);
                b = midpoint + vec3(-0.5, 0.5, 0.0);
                c = midpoint + vec3(0.5, 0.5, 0.0);
                d = midpoint + vec3(0.5, -0.5, 0.0);
            }
            if(plane.y && plane.z) {
                a = midpoint + vec3(0.0, -0.5, -0.5);
                b = midpoint + vec3(0.0, -0.5, 0.5);
                c = midpoint + vec3(0.0, 0.5, 0.5);
                d = midpoint + vec3(0.0, 0.5, -0.5);
            }
            if(plane.x && plane.z) {
                a = midpoint + vec3(-0.5, 0.0, -0.5);
                b = midpoint + vec3(-0.5, 0.0, 0.5);
                c = midpoint + vec3(0.5, 0.0, 0.5);
                d = midpoint + vec3(0.5, 0.0, -0.5);
            }

            // Generate attributes (2 polygons)
            mesh->push_back(buildAttribute(a, f.voxel, f.ao[0], 0x00));
            mesh->push_back(buildAttribute(b, f.voxel, f.ao[1], 0x00));
            mesh->push_back(buildAttribute(c, f.voxel, f.ao[2], 0x00));

            mesh->push_back(buildAttribute(a, f.voxel, f.ao[0], 0x00));
            mesh->push_back(buildAttribute(d, f.voxel, f.ao[3], 0x00));
            mesh->push_back(buildAttribute(c, f.voxel, f.ao[2], 0x00));
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
                    // Air to search from
                    s.push(c_dash); 
                } else {
                    // Face found
                    f.push_back({c, c_dash, voxels[idx(c_dash)]}); 
                }
            }
        }

        return f;
    }

    vector<Face>
    gridSearchVisibility(array<char, N_VOXELS> &chunk) {
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
        return faces;
    }

    vector<Face> 
    pruneFaces(vector<Face> &faces, NeighbourData &neighbourData) {
        vector<Face> pruned;
        for(auto &face : faces) {
            if(onBoundary(face.to) && boundaryOcclusion(face, neighbourData))
                continue;
            pruned.push_back(face);
        }
        return pruned;
    }
}

NeighbourData extractBoundaries(VoxelsImpl &v, ivec2 chunkCoordinate)
{
    // Populates NeighbourData for chunk at _chunkCoordinate_ at the instant the function is called
    NeighbourData data{};

    auto takePlane = [](array<char, SIZE_XY * SIZE_Z> &slice, array<char, N_VOXELS> &neighbourChunk, bool xzPlane, int plane) {
        // xzPlane defines whether scanning across X-Z or Y-Z plane defined by: 
        // x|y = plane in neighbourChunk
        for(int minor = 0; minor < SIZE_XY; minor++) {
            for(int z = 0; z < SIZE_Z; z++) {
                auto p = ivec3(xzPlane ? minor : plane, xzPlane ? plane : minor, z);
                slice[SIZE_XY * z + minor] = neighbourChunk[idx(p)];
            }
        }
    };
    auto n = chunkCoordinate + ivec2(0, 1);
    auto s = chunkCoordinate + ivec2(0, -1);
    auto e = chunkCoordinate + ivec2(1, 0);
    auto w = chunkCoordinate + ivec2(-1, 0);

    if(v.chunkDisplayed(n) )
        takePlane(data.north, v.getChunk(n)->voxelData(), true, 0);
    if(v.chunkDisplayed(s))
        takePlane(data.south, v.getChunk(s)->voxelData(), true, SIZE_XY-1);
    if(v.chunkDisplayed(e) )
        takePlane(data.east, v.getChunk(e)->voxelData(), false, 0);
    if(v.chunkDisplayed(w))
        takePlane(data.west, v.getChunk(w)->voxelData(), false, SIZE_XY-1);

    auto takeCorner = [](array<char, SIZE_Z> &corner, array<char, N_VOXELS> &neighbourChunk, ivec2 xy) {
        // Populates _corner_ with column of voxels at _xy_ from _neighbourChunk_
        for(int z = 0; z < SIZE_Z; z++)
            corner[z] = neighbourChunk[idx(ivec3(xy.x, xy.y, z))];
    };

    auto sw = chunkCoordinate + ivec2(-1, -1);
    auto nw = chunkCoordinate + ivec2(-1, 1);
    auto ne = chunkCoordinate + ivec2(1, 1);
    auto se = chunkCoordinate + ivec2(1, -1);

    if(v.chunkDisplayed(sw) )
        takeCorner(data.cornerSW, v.getChunk(sw)->voxelData(), ivec2(SIZE_XY-1, SIZE_XY-1));
    if(v.chunkDisplayed(nw) )
        takeCorner(data.cornerNW, v.getChunk(nw)->voxelData(), ivec2(SIZE_XY-1, 0));
    if(v.chunkDisplayed(ne) )
        takeCorner(data.cornerNE, v.getChunk(ne)->voxelData(), ivec2(0, 0));
    if(v.chunkDisplayed(se) )
        takeCorner(data.cornerSE, v.getChunk(se)->voxelData(), ivec2(0, SIZE_XY-1));

    return data;
}

std::shared_ptr<VoxelMesh> buildMeshDFS(array<char, N_VOXELS> chunk, NeighbourData neighbourData)
{
    // Generate list of vertices by DFS
    auto start = ivec3(-1, -1, 0);
    auto faces = DFS_visibility(start, chunk);
    auto pruned = pruneFaces(faces, neighbourData);

    // Ambient Occlusion
    populateAOData(chunk, pruned, neighbourData);

    return facesToPolygons(pruned);
}

std::shared_ptr<VoxelMesh> buildMeshGridSearch(array<char, N_VOXELS> chunk, NeighbourData neighbourData) {
    // Generate list of vertices by grid search
    auto faces = gridSearchVisibility(chunk);
    auto pruned = pruneFaces(faces, neighbourData);
    
    // Ambient Occlusion
    populateAOData(chunk, pruned, neighbourData);

    return facesToPolygons(pruned);
}