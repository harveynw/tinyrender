# tinyrender

[Docs / Usage / Examples](https://harveynw.github.io/tinyrender/) <<< Site (work in progress)

Minimalist C++ WebGPU Rendering Engine.

This might be cool if you just want to draw 3D scenes but don't want to touch graphics APIs nor the bloat of something like Unity.

### Current State

(Development screenshot) Phong lighting, OBJ files, fourier wave simulation:

![Screenshot](img/screenshot.png)

### Building and running locally

Just creates a test scene with an interactive camera view
```zsh
cmake -S . -B build
cmake --build build
build/App
```
Working with emscripten:
```zsh
emcmake cmake -B build-web
cmake --build build-web
```

### TODO
- More primitives (spheres, tubes, planes)
- Accelerated voxel rendering
- Material API

### References

The library relies heavily on the infrastructure provided in the excellent tutorial series by @eliemichel:

[Learn WebGPU : For native graphics in C++](https://eliemichel.github.io/LearnWebGPU/)