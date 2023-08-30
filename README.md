# tinyrender

Originally following [Learn WebGPU](https://eliemichel.github.io/LearnWebGPU/) by @eliemichel.
Turned into a sort of goal to build a barebones graphics engine for C++.
The wrapper provided in the series does the heavy lifting to make this cross-platform supported (including WASM).

This might be cool if you just want to draw 3D scenes but don't want to touch graphics APIs nor the bloat of something like Unity.

##### Current State

Rudimentary triangle rendering and transformations working on M1 Pro Mac:

![Screenshot](img/screenshot_old.png)

##### TODO
- More primitives (spheres, tubes, planes)
- Accelerated voxel rendering
- More lighting (skybox, point lights)
- Fake source-engine like water
