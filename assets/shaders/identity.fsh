#version 330 core

uniform sampler2D GB_COLOR;
// uniform sampler2D GB_NORMAL;
// uniform sampler2D GB_SURFACE;
// uniform sampler2D GB_LIGHT;

in vec2 screen_pos;

layout (location = 0) out vec4 FragColor;

void main() {
    gl_FragColor = vec4(texture(GB_COLOR, screen_pos).xyz, 1.0);
}
