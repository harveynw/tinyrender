# tinyrender

Originally following [Learn WebGPU](https://eliemichel.github.io/LearnWebGPU/) by @eliemichel.
Turned into a sort of goal to build a barebones graphics engine for C++.

Use of WebGPU and the wrapper provided in the series does the heavy lifting to provide cross-platform support (including WASM).

This might be cool if you just want to draw 3D scenes but don't want to touch graphics APIs nor the bloat of something like Unity.

### Current State

Rudimentary triangle rendering and transformations working on M1 Pro Mac:

![Screenshot](img/screenshot_old.png)

### Building and running locally

Just creates a test scene with an interactive camera view
```zsh
cmake -S . -B build
cmake --build build
build/App
```

### TODO
- More primitives (spheres, tubes, planes)
- Accelerated voxel rendering
- More lighting (skybox, point lights)
- Fake source-engine like water
