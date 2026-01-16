#version 330 core

out vec4 FragColor;

in vec3 vNormal;
in vec3 vFragPos;
in vec2 vTexCoord;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;  
};

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main() {
    vec3 ambient = light.ambient * texture(material.diffuse, vTexCoord).rgb;

    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(light.position - vFragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, vTexCoord).rgb;

    vec3 viewDir = normalize(viewPos - vFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * (1.0 - texture(material.specular, vTexCoord).rgb);

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}