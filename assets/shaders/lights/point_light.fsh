#version 330 core

uniform sampler2D GB_COLOR;
uniform sampler2D GB_NORM_DEPTH;
uniform sampler2D GB_SURFACE;
uniform sampler2D GB_FINAL;

uniform vec3 light_color;

uniform mat4 camera_inv;
uniform mat4 object_inv;

uniform vec3 object_pos;
uniform vec3 camera_pos;

in vec2 screen_pos;

layout(location = 0) out vec4 AlbedoOutput;
layout(location = 1) out vec4 NormalDepthOutput;
layout(location = 2) out vec4 SurfaceOutput;
layout(location = 3) out vec4 FinalOutput;

const float SPREAD = 3.0;

float brdf(vec3 light_dir, vec3 camera_dir, vec3 normal, float roughness,
           float specular) {
    float brightness = 0.0;

    vec3 bisector = normalize(light_dir) + normalize(camera_dir);
    float specular_amount = -dot(normalize(bisector), normalize(normal));
    float amount = -dot(normalize(light_dir), normalize(normal));

    float dist = length(light_dir);

    if (amount < 0.0) return 0.0;

    brightness += amount / (1.0 + dist * dist / SPREAD);

    if (specular_amount > 0.0) {
        brightness += pow(specular_amount, 1.0 / roughness) * specular /
                      (1.0 + dist * dist / SPREAD);
    }

    return brightness;
}

const float NEAR_PLANE = 2.0;
const float FAR_PLANE = 200.0;

void main() {
    vec4 normal_depth = texture(GB_NORM_DEPTH, screen_pos);
    vec4 surface = texture(GB_SURFACE, screen_pos);

    AlbedoOutput = texture(GB_COLOR, screen_pos);
    NormalDepthOutput = normal_depth;
    SurfaceOutput = surface;

    vec4 prev_final = texture(GB_FINAL, screen_pos);
    vec4 albedo = texture(GB_COLOR, screen_pos);
    float ao = texture(GB_SURFACE, screen_pos).z;

    vec3 normal = normal_depth.xyz * 2.0 - 1.0;

    if (length(normal) <= 0.9) {
        FinalOutput = prev_final;
        return;
    }

    vec2 centered_scr = screen_pos.xy * 2.0 - 1.0;

    vec4 screen_space = vec4(centered_scr, normal_depth.w * 2.0 - 1.0, 1.0);
    vec4 world_space = camera_inv * screen_space;
    world_space /= world_space.w;

    vec3 light_vector = world_space.xyz - object_pos;
    vec3 camera_vector = world_space.xyz - camera_pos;

    float brightness =
        brdf(light_vector, camera_vector, normal, surface.x, surface.y);

    FinalOutput =
        vec4(prev_final.xyz + light_color * brightness * albedo.xyz * ao, 1.0);
}
