#version 330 core

uniform sampler2D GB_COLOR;
uniform sampler2D GB_NORM_DEPTH;
uniform sampler2D GB_SURFACE;
uniform sampler2D GB_FINAL;

uniform vec3 light_color;

in vec2 screen_pos;

layout(location = 0) out vec4 AlbedoOutput;
layout(location = 1) out vec4 NormalDepthOutput;
layout(location = 2) out vec4 SurfaceOutput;
layout(location = 3) out vec4 FinalOutput;

void main() {
    AlbedoOutput = texture(GB_COLOR, screen_pos);
    NormalDepthOutput = texture(GB_NORM_DEPTH, screen_pos);
    SurfaceOutput = texture(GB_SURFACE, screen_pos);

    vec4 prev_final = texture(GB_FINAL, screen_pos);
    vec4 albedo = texture(GB_COLOR, screen_pos);
    float ao = texture(GB_COLOR, screen_pos).z;

    FinalOutput = vec4(prev_final.rgb + albedo.rgb * light_color, 1.0);

    // FinalOutput = vec4(1.0, 0.5, 0.0, 1.0);
}
