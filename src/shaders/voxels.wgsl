R""(

struct VertexInput {
    @location(0) position: vec3f,
    @location(1) data: f32,
};
struct VertexOutput {
    @builtin(position) position: vec4f,
    @location(0) color: vec3f,
};

struct MyUniforms {
    projectionMatrix: mat4x4f,
    viewMatrix: mat4x4f,
    cameraWorldPosition: vec3f,
};

struct LightingUniforms {
    directions: array<vec4<f32>, 2>,
    colors: array<vec4<f32>, 2>,
    ambient: f32,
};

@group(0) @binding(0) var<uniform> uMyUniforms: MyUniforms;
@group(0) @binding(1) var<uniform> uLighting: LightingUniforms;

@group(1) @binding(0) var<uniform> globalModelMatrix: mat4x4f;
@group(1) @binding(1) var<uniform> chunkModelMatrix: mat4x4f;

fn unifToColor(u: f32) -> vec3f {
    let t: f32 = u * 3.14 * 10.0f;

    let r: f32 = (sin(t) + 1.0f)/2.0f;
    let g: f32 = (cos(t) + 1.0f)/2.0f;
    let b: f32 = (sin(t) + 1.0f)/2.0f;

    return vec3f(r, g, b);
}

@vertex
fn vs_main(in: VertexInput) -> VertexOutput {
    let world: vec4f = globalModelMatrix * chunkModelMatrix * vec4f(in.position, 1.0);

    // Forward to fragment shader
    var out: VertexOutput;
    out.position = uMyUniforms.projectionMatrix * uMyUniforms.viewMatrix * world;
    
    // Decide color of voxel
    let data: vec4<f32> = unpack4x8unorm(bitcast<u32>(in.data));
    out.color = unifToColor(data.x); //vec3f(0.0f, 0.0f, data.x);

    return out;
}

@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4f {
    let baseColor = in.color;
    return vec4f(baseColor, 1.0);
}
)""