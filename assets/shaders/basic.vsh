#version 330 core

layout (location = 0) in vec3 i_position;
layout (location = 1) in vec3 i_normal;
layout (location = 2) in vec2 i_uv;

out vec3 local_pos;
out vec3 world_pos;
out vec3 normal;
out vec2 uv;

uniform mat4 obj_tform;
uniform mat4 projection;

void main() {
    gl_Position = projection * obj_tform * vec4(i_position, 1.0);
    world_pos = (obj_tform * vec4(i_position, 1.0)).xyz;
    local_pos = i_position;
    normal = (obj_tform * vec4(i_normal, 0.0)).xyz;
    uv = i_uv;
}

