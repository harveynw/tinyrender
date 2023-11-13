#include "load_shader.hpp"

ShaderModule
loadShaderModule(std::string name, Device device) {
    /*
    std::ifstream file(path);
    if (!file.is_open())
        throw std::runtime_error("Couldn't open shader path " + path);

    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    std::string shaderSource(size, ' ');
    file.seekg(0);
    file.read(shaderSource.data(), size);
    */

    std::string shaderSource;
    if(name == "basic_mvp_color")
        shaderSource = basic_mvp_color;
    if(name == "basic_mvp_texture")
        shaderSource = basic_mvp_texture;
    if(name == "voxels")
        shaderSource = voxels;
    if(name == "waves")
        shaderSource = waves;
    if(shaderSource.empty())
        throw std::runtime_error("No shader source found");

    ShaderModuleWGSLDescriptor shaderCodeDesc{};
    shaderCodeDesc.chain.next = nullptr;
    shaderCodeDesc.chain.sType = SType::ShaderModuleWGSLDescriptor;
    shaderCodeDesc.code = shaderSource.c_str();
    ShaderModuleDescriptor shaderDesc{};
#ifdef WEBGPU_BACKEND_WGPU
    shaderDesc.hintCount = 0;
    shaderDesc.hints = nullptr;
#endif
    shaderDesc.nextInChain = &shaderCodeDesc.chain;
    return device.createShaderModule(shaderDesc);
}
