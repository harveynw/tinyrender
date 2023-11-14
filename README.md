# tinyrender

Minimalist C++ Cross-Platform Rendering Engine built using WebGPU.

[Docs / Usage / Examples](https://harveynw.github.io/tinyrender/) <<< Site (work in progress)

This might be cool if you just want to draw 3D scenes but don't want to touch hardcore graphics APIs nor the bloat of fully functional game engines.

### Current State

(Development screenshot) Phong lighting, OBJ files, fourier wave simulation:

![Screenshot](img/screenshot.png)

Currently testing on and building for:
- ARM macOS 
- x86_64 Ubuntu Linux

### Building and running locally

View the [install instructions](https://harveynw.github.io/tinyrender/install.html) for integrating with your CMake project. 

To see a test scene:
```zsh
cmake -S . -B build
cmake --build build
./build/examples/TestScene/TestScene
```

### Roadmap

- More geometric primitives (spheres, tubes, planes)
- Finishing Voxel Engine API
- More material functionality (including BRDFs) 
- Animation

### References

The library relies heavily on the infrastructure provided in the excellent tutorial series by @eliemichel:

[Learn WebGPU : For native graphics in C++](https://eliemichel.github.io/LearnWebGPU/)

### Contributors
[@davenewham](https://github.com/davenewham)