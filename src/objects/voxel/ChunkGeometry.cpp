#include "ChunkGeometry.hpp"


int idx(ivec3 c)
{
    // Voxel coordinates to array index
    return c.x + SIZE_XY*c.y + SIZE_XY*SIZE_XY*c.z;
}

int 
hash(ivec3 c) {
    // Hash coordinates, note different to idx due to boundary search requirement
    return (c.x+1) + (SIZE_XY+2)*(c.y+1) + (SIZE_XY+2)*(SIZE_XY+2)*(c.z+1);
}

ivec3 
coord(int i) {
    // Unpack array index into voxel coordinates
    int x = i % SIZE_XY;
    int y = (i / SIZE_XY)%SIZE_XY;
    int z = i / (SIZE_XY*SIZE_XY);
    return ivec3(x, y, z);
}

bool
inBounds(ivec3 &c) {
    bool x = 0 <= c.x && c.x < SIZE_XY;
    bool y = 0 <= c.y && c.y < SIZE_XY;
    bool z = 0 <= c.z && c.z < SIZE_Z;
    return x && y && z;
}

vector<ivec3> 
adjacentCoordinates(ivec3 &c) {
    // Returns adjacent integer coordinates, including a unit padding around the chunk to aid search
    vector<ivec3> adj;
    if(-1 <= c.x - 1)
        adj.push_back(c - ivec3(1, 0, 0));
    if(c.x + 1 < SIZE_XY + 1)
        adj.push_back(c + ivec3(1, 0, 0));
    if(-1 <= c.y - 1)
        adj.push_back(c - ivec3(0, 1, 0));
    if(c.y + 1 < SIZE_XY + 1)
        adj.push_back(c + ivec3(0, 1, 0));
    if(-1 <= c.z - 1)
        adj.push_back(c - ivec3(0, 0, 1));
    if(c.z + 1 < SIZE_Z + 1)
        adj.push_back(c + ivec3(0, 0, 1));
    return adj;
}


bool 
isTransparent(ivec3 &c, array<char, N_VOXELS> &voxels) {
    return voxels[idx(c)] == 0x00;
}

bool 
onBoundary(ivec3 &c) {
    return c.x == 0 || c.x == SIZE_XY-1 || c.y == 0 || c.y == SIZE_XY-1 || c.z == 0 || c.z == SIZE_Z-1;
}

ivec3 
wrapCoordinate(ivec3 &c) {
    return ivec3(MOD(c.x, SIZE_XY), MOD(c.y, SIZE_XY), c.z);
}

string 
serializeChunkCoord(ivec2 coord) {
    std::ostringstream ss;
    ss << coord.x << "," << coord.y;
    return ss.str();
}

bool 
visible(ivec2 chunkA, ivec2 chunkB) {
    int dx = abs(chunkA.x - chunkB.x);
    int dy = abs(chunkA.y - chunkB.y);
    return dx <= VISIBILITY_DISTANCE && dy <= VISIBILITY_DISTANCE;
}

vector<ivec2> 
visibleFrom(ivec2 chunk)
{
    vector<ivec2> visible;
    for(int i = -VISIBILITY_DISTANCE; i <= VISIBILITY_DISTANCE; i++) {
        for(int j = -VISIBILITY_DISTANCE; j <= VISIBILITY_DISTANCE; j++)
            visible.emplace_back(i+chunk.x, j+chunk.y);
    }
    return visible;
}
