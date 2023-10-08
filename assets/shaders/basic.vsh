#version 330 core

layout (location = 0) in vec3 i_position;
layout (location = 1) in vec3 i_normal;
layout (location = 2) in vec2 i_uv;

out vec3 pos;
out vec3 normal;
out vec2 uv;

uniform mat4 transform;

void main() {
    gl_Position = transform * vec4(i_position, 1.0);
    pos = i_position;
    normal = i_normal;
    uv = i_uv;
}

