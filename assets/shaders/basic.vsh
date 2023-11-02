#version 330 core

layout (location = 0) in vec3 i_position;
layout (location = 1) in vec3 i_normal;
layout (location = 2) in vec2 i_uv;
layout (location = 3) in vec3 i_tangent;

out vec3 local_pos;
out vec3 world_pos;
out vec4 normal_depth;
out vec2 uv;
out vec3 tangent;

uniform mat4 obj_tform;
uniform mat4 projection;

void main() {
    vec4 position = projection * obj_tform * vec4(i_position, 1.0);
    gl_Position = position;
    world_pos = (obj_tform * vec4(i_position, 1.0)).xyz;
    local_pos = i_position;
    normal_depth = vec4((obj_tform * vec4(i_normal, 0.0)).xyz, position.z);
    uv = i_uv;
    tangent = i_tangent;
}

