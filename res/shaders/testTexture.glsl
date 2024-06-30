#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoords;

out vec2 v_texCoords;

uniform mat4 u_proj;
uniform mat4 u_model;

void main() {
    gl_Position = u_proj * u_model * position;
    v_texCoords = texCoords;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_texCoords;

uniform sampler2D u_Texture;

void main() {
    vec4 texColor = texture(u_Texture, v_texCoords);
    color = texColor;
}
