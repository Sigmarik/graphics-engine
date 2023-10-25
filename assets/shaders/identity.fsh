#version 330 core

uniform sampler2D GB_FINAL;

in vec2 screen_pos;

void main() { gl_FragColor = vec4(texture(GB_FINAL, screen_pos).xyz, 1.0); }
