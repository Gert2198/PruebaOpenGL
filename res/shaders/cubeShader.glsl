#shader vertex
#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;

out vec3 normal;
out vec3 fragPos;

uniform mat4 u_model;
uniform mat4 u_MVP; // Model view projection matrix

void main() {
    gl_Position = u_MVP * vec4(aPosition, 1.0);
    fragPos = vec3(u_model * vec4(aPosition, 1.0));
    normal = aNormal;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 normal;
in vec3 fragPos;

uniform vec3 u_cameraPos;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
  
uniform Material u_material;

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light u_light;  

void main() {
    vec3 ambient = u_light.ambient * u_material.ambient;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(u_light.position - fragPos);  
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = u_light.diffuse * diff * u_material.diffuse;

    vec3 viewDir = normalize(u_cameraPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess); // El exponente cambia la amplitud del reflejo
    vec3 specular = u_light.specular * (u_material.specular * spec);  

    vec3 result = ambient + diffuse + specular;
    color = vec4(result, 1.0);
}
