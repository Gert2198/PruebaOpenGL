#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoords;

out vec2 v_texCoords;

uniform mat4 u_MVP; // Model view projection matrix

void main() {
    gl_Position = u_MVP * position;
    v_texCoords = texCoords;
}