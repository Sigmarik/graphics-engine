#version 330 core

uniform sampler2D albedo;

in vec3 local_pos;
in vec3 world_pos;
in vec3 normal;
in vec2 uv;

layout(location = 0) out vec4 AlbedoOutput;

void main() { AlbedoOutput = vec4(texture(albedo, uv).xyz, 1.0); }
