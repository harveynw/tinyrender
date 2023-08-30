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

@group(0) @binding(0) var<uniform> uMyUniforms: MyUniforms;
@group(0) @binding(1) var<uniform> modelMatrix: mat4x4f;
@group(0) @binding(2) var texture: texture_2d<f32>;
@group(0) @binding(3) var textureSampler: sampler;


@vertex
fn vs_main(in: VertexInput) -> VertexOutput {
    // Forward to fragment shader
    var out: VertexOutput;
    out.position = uMyUniforms.projectionMatrix * uMyUniforms.viewMatrix * modelMatrix * vec4f(in.position, 1.0);
    out.color = in.color;
    out.normal = (modelMatrix * vec4f(in.normal, 0.0)).xyz; // wrt to model matrix but not camera
    //out.uv = in.uv;
    out.uv = 5.0 * in.uv - 0.5;
    return out;
}

@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4f {
    // TODO: Gamma correction
    // https://eliemichel.github.io/LearnWebGPU/basic-3d-rendering/input-geometry/loading-from-file.html
    //let texelCoords = vec2i(in.uv * vec2f(textureDimensions(texture)));
    let color = textureSample(texture, textureSampler, in.uv).rgb;

    let normal = normalize(in.normal);
    let lightDirection = vec3f(0.5, -0.9, 0.1);
    let shading = dot(lightDirection, normal);

    //return vec4f(color * shading, 1.0);
    return vec4f(color, 1.0);
}