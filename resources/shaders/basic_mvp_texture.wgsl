struct VertexInput {
    @location(0) position: vec3f,
    @location(1) normal: vec3f,
    @location(2) color: vec3f,
    @location(3) uv: vec2f,
};
struct VertexOutput {
    @builtin(position) position: vec4f,
    @location(0) normal: vec3f,
    @location(1) color: vec3f,
    @location(2) uv: vec2f,
};

struct MyUniforms {
    projectionMatrix: mat4x4f,
    viewMatrix: mat4x4f,
    color: vec4f,
    time: f32,
};

struct LightingUniforms {
    directions: array<vec4<f32>, 2>,
    colors: array<vec4<f32>, 2>,
}

@group(0) @binding(0) var<uniform> uMyUniforms: MyUniforms;
@group(0) @binding(1) var<uniform> modelMatrix: mat4x4f;
@group(0) @binding(2) var<uniform> uLighting: LightingUniforms;
@group(0) @binding(3) var texture: texture_2d<f32>;
@group(0) @binding(4) var textureSampler: sampler;


@vertex
fn vs_main(in: VertexInput) -> VertexOutput {
    // Forward to fragment shader
    var out: VertexOutput;
    out.position = uMyUniforms.projectionMatrix * uMyUniforms.viewMatrix * modelMatrix * vec4f(in.position, 1.0);
    out.color = in.color;
    out.normal = (modelMatrix * vec4f(in.normal, 0.0)).xyz; // wrt to model matrix but not camera
    out.uv = in.uv;

    return out;
}

@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4f {
    // TODO: Gamma correction
    // https://eliemichel.github.io/LearnWebGPU/basic-3d-rendering/input-geometry/loading-from-file.html

    // Compute shading
    let normal = normalize(in.normal);
    var shading = vec3<f32>(0.0);
    for (var i: i32 = 0; i < 2; i++) {
        let direction = normalize(uLighting.directions[i].xyz);
        let color = uLighting.colors[i].rgb;
        shading += max(0.0, dot(direction, normal)) * color;
    }

    // Sample texture
    let baseColor = textureSample(texture, textureSampler, in.uv).rgb;

    // Combine texture and lighting
    let color = baseColor * shading;

    return vec4f(color * shading, 1.0);
}