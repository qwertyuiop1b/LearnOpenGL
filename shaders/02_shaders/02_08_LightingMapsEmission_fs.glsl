#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 vNormal;
in vec3 fragPos;
in vec2 texCoords;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform float uTime;

out vec4 fragColor;

void main() {
    // 环境光
    vec3 ambient = light.ambient * texture(material.diffuse, texCoords).rgb;

    // 漫反射
    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, texCoords).rgb;

    // 镜面光
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, texCoords).rgb;

    // emission
    // texture(material.specular, texCoords).rgb 木箱中间为黑色
    vec3 emission =  texture(material.emission, texCoords + vec2(0.0, uTime)).rgb * floor(vec3(1.0f) - texture(material.specular, texCoords).rgb);


    vec3 result = ambient + diffuse + specular + emission;
    fragColor = vec4(result, 1.0);
}
