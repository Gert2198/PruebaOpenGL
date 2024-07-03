#shader vertex
#version 330 core

layout(location = 0) in vec3 position;

out float v_color;

uniform mat4 u_MVP; // Model view projection matrix

void main() {
    gl_Position = u_MVP * vec4(position.xyz, 1.0);
    v_color = length(position)/ 1.5;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in float v_color;

void main() {
    color = vec4(v_color, v_color, v_color, 1.0);
}
