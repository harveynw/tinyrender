struct VertexInput {
    @location(0) position: vec3f,
    @location(1) normal: vec3f,
    @location(2) color: vec3f,
};
struct VertexOutput {
    @builtin(position) position: vec4f,
    @location(0) normal: vec3f,
    @location(1) color: vec3f,
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
@group(1) @binding(1) var<uniform> color: vec3f;

@vertex
fn vs_main(in: VertexInput) -> VertexOutput {
    var pos = in.position;

    let worldPosition: vec4f = modelMatrix * vec4f(pos, 1.0);

    // Forward to fragment shader
    var out: VertexOutput;
    out.position = uMyUniforms.projectionMatrix * uMyUniforms.viewMatrix * modelMatrix * vec4f(pos, 1.0);
    out.color = in.color;
    out.normal = (modelMatrix * vec4f(in.normal, 0.0)).xyz; // wrt to model matrix but not camera
    out.viewDirection = uMyUniforms.cameraWorldPosition - worldPosition.xyz;

    return out;
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
    let normal = normalize(in.normal);

    // Color passed by attribute buffer
    let baseColor = in.color;

    let kd = 0.5; // strength of the diffuse effect
    let ks = 0.8; // strength of the specular effect
    let color = uLighting.ambient * baseColor + kd * diffuse(normal) * baseColor + ks * specular(normal, in.viewDirection);

    return vec4f(color, 1.0);
}