#shader vertex
#version 330 core

layout(location = 0) in vec2 position;

uniform mat4 u_MVP; // Model view projection matrix

void main() {
    gl_Position = u_MVP * vec4(position.xy, 0.0, 1.0);
    v_texCoords = texCoords;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_Color;

void main() {
    color = u_Color;
}
