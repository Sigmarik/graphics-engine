#version 330 core

uniform sampler2D GB_COLOR;
uniform sampler2D GB_NORM_DEPTH;
uniform sampler2D GB_SURFACE;
uniform sampler2D GB_FINAL;

uniform sampler2D noise;

uniform mat4 camera_inv;
uniform mat4 object_inv;

uniform mat4 object_pos;

uniform float time_to_live;
uniform float decay;

in vec2 screen_pos;

layout(location = 0) out vec4 AlbedoOutput;
layout(location = 1) out vec4 NormalDepthOutput;
layout(location = 2) out vec4 SurfaceOutput;
layout(location = 3) out vec4 FinalOutput;

float get_dencity(vec3 position) {
    float dencity = 1.0 - length(position);
    dencity *= texture(noise, position.xz * 0.5 + 0.5).x;

    return clamp(dencity * decay * 3.0, 0.0, 1.0);

    if (dencity < 1.0 - decay) return 0.0;
    return 1.0;
}

void main() {
    vec4 normal_depth = texture(GB_NORM_DEPTH, screen_pos);
    vec4 surface = texture(GB_SURFACE, screen_pos);

    NormalDepthOutput = normal_depth;
    SurfaceOutput = surface;
    FinalOutput = texture(GB_FINAL, screen_pos);

    vec4 albedo = texture(GB_COLOR, screen_pos);

    vec3 normal = normal_depth.xyz * 2.0 - 1.0;

    if (length(normal) <= 0.9) {
        AlbedoOutput = albedo;
        return;
    }

    vec2 centered_scr = screen_pos.xy * 2.0 - 1.0;

    vec4 screen_space = vec4(centered_scr, normal_depth.w * 2.0 - 1.0, 1.0);
    vec4 world_space = camera_inv * screen_space;
    world_space /= world_space.w;

    vec4 local_space = object_inv * world_space;
    local_space /= local_space.w;

    float dencity = get_dencity(local_space.xyz);

    AlbedoOutput = mix(albedo, vec4(0.8, 0.1, 0.1, 1.0), dencity);
}
