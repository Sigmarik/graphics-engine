#version 330 core

uniform sampler2D albedo;

in vec3 pos;
in vec3 normal;
in vec2 uv;

out vec4 FragColor;

void main() { FragColor = vec4(texture(albedo, uv).xyz, 1.0); }