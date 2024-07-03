#shader vertex
#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 u_MVP; // Model view projection matrix

void main() {
    gl_Position = u_MVP * vec4(position.xyz, 1.0);
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

void main() {
    color = vec4(0.0, 0.0, 0.0, 1.0);
}
