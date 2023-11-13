R""(

struct VertexInput {
    @location(0) position: vec3f,
    @location(1) normal: vec3f,
    @location(2) color: vec3f,
    @location(3) uv: vec2f,
};
struct VertexOutput {
    @builtin(position) position: vec4f,
    @location(0) color: vec3f,
    @location(1) uv: vec2f,
    @location(2) viewDirection: vec3f,
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

@group(1) @binding(0) var<uniform> modelMatrix: mat4x4f;
@group(1) @binding(1) var heightmap: texture_2d<f32>;
@group(1) @binding(2) var textureSampler: sampler;
@group(1) @binding(3) var<uniform> maxDisplacement: f32;
@group(1) @binding(4) var<uniform> wavesColor: vec3f;

@vertex
fn vs_main(in: VertexInput) -> VertexOutput {
    var out: VertexOutput;

    var pos = in.position;
    pos.z += maxDisplacement * textureSampleLevel(heightmap, textureSampler, in.uv, 0.0).x;

    let worldPosition: vec4f = modelMatrix * vec4f(pos, 1.0);

    out.position = uMyUniforms.projectionMatrix * uMyUniforms.viewMatrix * worldPosition;
    out.color = wavesColor;
    out.uv = in.uv;
    out.viewDirection = uMyUniforms.cameraWorldPosition - worldPosition.xyz;

    return out;
}

fn displacement(uv: vec2f) -> f32 {
    //return 0.5 + 0.5 * textureSample(heightmap, textureSampler, uv).x;
    return textureSample(heightmap, textureSampler, uv).x;
}

fn computeNormal(uv: vec2f) -> vec3f {
  	// Finite difference normal
    var sm: f32 = 0.002;

    var R: f32 = maxDisplacement * displacement(uv + vec2f(sm, 0.0));
    var L: f32 = maxDisplacement * displacement(uv + vec2f(-sm, 0.0));
    var T: f32 = maxDisplacement * displacement(uv + vec2f(0.0, sm));
    var B: f32 = maxDisplacement * displacement(uv + vec2f(0.0, -sm));

    // Take cross product with z always positive
    return normalize(vec3f((L-R)/(2.0*sm), (B-T)/(2.0*sm), 1.0));
}

fn diffuse(normal: vec3f) -> vec3f {
    var shading = vec3f(0.0);
    for (var i: i32 = 0; i < 2; i++) {
        let direction = normalize(uLighting.directions[i].xyz);
        let color = uLighting.colors[i].rgb;
        shading += max(0.0, dot(direction, normal)) * color;
    }
    return shading;
}

fn specular(normal: vec3f, viewDirection: vec3f) -> f32 {
    var total_specular: f32 = 0.0;
    let V = normalize(viewDirection);

    for (var i: i32 = 0; i < 2; i++) {
        // Maximal reflection angle
        let R = reflect(-normalize(uLighting.directions[i].xyz), normal);

        let RoV = max(0.0, dot(R, V)); // Cosine angle between viewing and reflection
        let hardness = 3.5;//2.0;
        total_specular += pow(RoV, hardness);
    }

    return total_specular;
}

@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4f {
    var normal: vec3f = computeNormal(in.uv);
    let baseColor = in.color;
        
    let kd = 0.5; // strength of the diffuse effect
    let ks = 0.8; // strength of the specular effect
    let color = uLighting.ambient * baseColor + kd * diffuse(normal) * baseColor + ks * specular(normal, in.viewDirection);

    // Show the computed color with lighting
    return vec4f(color, 1.0);

    // Show the normal map
    //return vec4f(abs(normal.xy), normal.z, 1.0);

    // Show the UV mapping
    //return vec4f(in.uv, 1.0, 1.0);

    // Show the displacement map as greyscale
    //return vec4f(vec3f(displacement(in.uv)), 1.0);
}

)""