#pragma once

#include <memory>

#include "ObjectImpl.hpp"
#include "objects/Voxels.hpp"
#include "objects/voxel/Chunk.hpp"
#include "voxel/ChunkImpl.hpp"
#include "State.hpp"
#include "loaders/Shapes.hpp"
#include "ObjectResources.hpp"
#include "voxel/BuildMeshQueue.hpp"


class VoxelsImpl : public ObjectImpl {
    // Handles its own rendering 
    using ObjectImpl::setColor;
    using ObjectImpl::setTexture;

    std::shared_ptr<ModelMatrixUniform> globalModelMatrix;
    std::shared_ptr<Texture2D::CharColorTexture> voxelColors;

    std::map<std::string, std::shared_ptr<tinyrender::Chunk>> map;

    static ChunkImpl* impl(std::shared_ptr<tinyrender::Chunk> &chunk);
public:
    VoxelsImpl() = default;

    void onInit(Context *c, Scene *s) override;
    void onUpdate(State &state, float dt) override;
    void onDraw(wgpu::RenderPassEncoder &renderPass, int vertexBufferSlot, int bindGroupSlot) override;
    void onRemove() override;

    ObjectPipeline currentTargetPipeline() override;

    std::shared_ptr<ModelMatrixUniform> modelMatrix() const override;
    std::shared_ptr<Texture2D::CharColorTexture> colorTexture();

    // Chunk API
    std::shared_ptr<tinyrender::Chunk> getChunk(ivec2 chunkCoordinate);
    bool chunkTracked(ivec2 coord); // Instance of chunk exists
    bool chunkDisplayed(ivec2 coord); // Chunk is being rendered
    std::vector<std::shared_ptr<tinyrender::Chunk>> visibleChunks();

    ~VoxelsImpl();
};