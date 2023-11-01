#version 330 core

uniform sampler2D GB_COLOR;
uniform sampler2D GB_NORM_DEPTH;
uniform sampler2D GB_SURFACE;
uniform sampler2D GB_FINAL;

in vec2 screen_pos;

layout(location = 0) out vec4 AlbedoOutput;
layout(location = 1) out vec4 NormalDepthOutput;
layout(location = 2) out vec4 SurfaceOutput;
layout(location = 3) out vec4 FinalOutput;

void main() {
    AlbedoOutput = texture(GB_COLOR, screen_pos);
    NormalDepthOutput = texture(GB_NORM_DEPTH, screen_pos);
    SurfaceOutput = texture(GB_NORM_DEPTH, screen_pos);

    vec3 final = texture(GB_FINAL, screen_pos).xyz;
    final = vec3(smoothstep(0.0, 1.0, final.r), smoothstep(0.0, 1.0, final.g),
                 smoothstep(0.0, 1.0, final.b));

    vec2 centered_pos = screen_pos * 2.0 - 1.0;

    float vignette_parabola = (1.0 - centered_pos.x * centered_pos.x) *
                              (1.0 - centered_pos.y * centered_pos.y);

    final *= 0.5 * (1.0 + pow(vignette_parabola, 1.0 / 5.0));

    FinalOutput = vec4(final, 1.0);
    // FinalOutput = vec4(vignette_parabola);
}
