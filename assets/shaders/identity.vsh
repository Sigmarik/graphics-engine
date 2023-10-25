#version 330 core

layout (location = 0) in vec2 i_position;

out vec2 screen_pos;

void main() {
    gl_Position = vec4(i_position, 0.0, 1.0);
    screen_pos = i_position / 2.0 - 0.5;
}

