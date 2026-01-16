#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct SpotLight {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;  

    float constant;
    float linear;
    float quadratic;

    float cutOff;
    float outerCutOff;
};

in vec3 vFragPos;
in vec3 vNormal;
in vec2 vTexCoord;

uniform vec3 viewPos;
uniform Material material;
uniform SpotLight light;

out vec4 FragColor;


void main() {
    vec3 ambient = light.ambient * texture(material.diffuse, vTexCoord).rgb;

    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(light.position - vFragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, vTexCoord).rgb;

    vec3 viewDir = normalize(viewPos - vFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * (texture(material.specular, vTexCoord).rgb);

    // 边缘处理
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon   = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0); 
    diffuse *= intensity;
    specular *= intensity;

    // 衰减
    float distance = length(light.position - vFragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);

    
    
}