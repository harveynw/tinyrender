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
};

struct ScalarUniform {
    scalar: f32
};

struct LightingUniforms {
    directions: array<vec4<f32>, 2>,
    colors: array<vec4<f32>, 2>,
}

@group(0) @binding(0) var<uniform> uMyUniforms: MyUniforms;
@group(0) @binding(1) var<uniform> uLighting: LightingUniforms;

@group(1) @binding(0) var<uniform> modelMatrix: mat4x4f;
@group(1) @binding(1) var heightmap: texture_2d<f32>;
@group(1) @binding(2) var textureSampler: sampler;
@group(1) @binding(3) var<uniform> maxDisplacement: f32;
@group(1) @binding(4) var<uniform> wavesColor: vec3f;

@vertex
fn vs_main(in: VertexInput) -> VertexOutput {
    var out: VertexOutput;

    // Displace according to heightmap, use textureSampleLevel which is available at vertex shader stage
  	var d_pos : vec3f = in.position;
  	d_pos.z += textureSampleLevel(heightmap, textureSampler, in.uv, 0.0).r * maxDisplacement;

  	// Finite difference normal
  	var grad: vec2f = textureSampleGrad(heightmap, textureSampler, in.uv, vec2f(0.01, 0.01), vec2f(0.01, 0.01)).xy;
  	var n: vec3f = normalize(vec3f(grad.xy, 1.0));

    out.position = uMyUniforms.projectionMatrix * uMyUniforms.viewMatrix * modelMatrix * vec4f(d_pos, 1.0);
    out.color = wavesColor;
    out.normal = (modelMatrix * vec4f(n, 0.0)).xyz;
    out.uv = in.uv;

    return out;
}

@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4f {
    // Compute shading
    let normal = normalize(in.normal);
    var shading = vec3<f32>(0.0);
    for (var i: i32 = 0; i < 2; i++) {
        let direction = normalize(uLighting.directions[i].xyz);
        let color = uLighting.colors[i].rgb;
        shading += max(0.0, dot(direction, normal)) * color;
    }

    // Combine base color uniform and lighting
    return vec4f(in.color * shading, 1.0);
}