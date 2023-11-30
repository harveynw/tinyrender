#include "Chunks.hpp"
#include "Chunk.hpp"

std::shared_ptr<Chunk> Chunks::getChunk(ivec2 chunkCoordinate)
{
    auto key = serializeChunkCoord(chunkCoordinate);
    if(map.count(key) > 0)
        return map.at(key);
    
    auto chunk = std::make_shared<Chunk>(c, s, *this, chunkCoordinate, globalModelMatrix);
    map[key] = chunk;
    return chunk;
}

bool Chunks::chunkTracked(ivec2 coord)
{
    return map.count(serializeChunkCoord(coord)) > 0;
}

bool Chunks::chunkDisplayed(ivec2 coord)
{
    return chunkTracked(coord) && getChunk(coord)->isVisible();
}
