struct VertexInput {
    @location(0) position: vec3f,
    @location(1) normal: vec3f,
    @location(2) color: vec3f,
};
struct VertexOutput {
    @builtin(position) position: vec4f,
    // The location here does not refer to a vertex attribute, it just means
    // that this field must be handled by the rasterizer.
    // (It can also refer to another field of another struct that would be used
    // as input to the fragment shader.)
    @location(0) normal: vec3f,
    @location(1) color: vec3f
};

struct MyUniforms {
    projectionMatrix: mat4x4f,
    viewMatrix: mat4x4f,
    color: vec4f,
    time: f32,
};

@group(0) @binding(0) var<uniform> uMyUniforms: MyUniforms;
@group(0) @binding(1) var<uniform> modelMatrix: mat4x4f;


@vertex
fn vs_main(in: VertexInput) -> VertexOutput {
    // Forward to fragment shader
    var out: VertexOutput;
    out.position = uMyUniforms.projectionMatrix * uMyUniforms.viewMatrix * modelMatrix * vec4f(in.position, 1.0);
    out.color = in.color;
    out.normal = (modelMatrix * vec4f(in.normal, 0.0)).xyz; // Change wrt to model matrix but not camera
    return out;
}

@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4f {
    // TODO: Gamma correction
    // https://eliemichel.github.io/LearnWebGPU/basic-3d-rendering/input-geometry/loading-from-file.html
    let normal = normalize(in.normal);
    let lightDirection = vec3f(0.5, -0.9, 0.1);
    let shading = dot(lightDirection, normal);
    let color = in.color * shading;
    return vec4f(color, 1.0);
}