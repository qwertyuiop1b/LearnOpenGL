#version 300 core

in vec2 vTexcoord;
in vec3 vNormal;

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
}

struct Material {
    Sampler diffuse;
    vec3 specular;
    uint shininess;
};

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main() {
    // ambient
    vec3 ambient = light.ambient * texture(material.diffuse, vTexcoord).rgb;

    // diffuse
    

}