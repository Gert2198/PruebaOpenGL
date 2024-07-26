#shader vertex
#version 330 core

layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec2 aTexCoords;

out vec2 texCoords;

uniform mat4 u_MVP;

void main() {
    gl_Position = u_MVP * vec4(aPosition, 0.0, 1.0);
    texCoords = aTexCoords;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 texCoords;

uniform vec3 u_color;
uniform bool u_hasTexture;
uniform sampler2D u_texture;

void main() {
    if (u_hasTexture) {
        vec4 texColor = texture(u_Texture, v_texCoords);
        color = texColor;
    } else {
        color = vec4(u_color, 1.0);
    }
}
