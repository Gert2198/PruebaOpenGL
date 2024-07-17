#shader vertex
#version 330 core

layout(location = 0) in vec2 aPosition;

uniform mat4 u_MVP;

void main() {
    gl_Position = u_MVP * vec4(aPosition, 0.0, 1.0);
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform vec3 u_color;

void main() {
    color = vec4(u_color, 1.0);
}
