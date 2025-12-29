#version 330 core

out vec4 FragColor;

in vec3 vNormal;
in vec3 vFragPos;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 viewPos;


void main() {
    // ambient
    float ambientStrength = 0.05;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 direction = normalize(lightPos - vFragPos);
    vec3 normal = normalize(vNormal);
    float diffuseStrength = max(dot(direction, normal), 0.0);
    vec3 diffuse = diffuseStrength * lightColor;

    // specular
    vec3 reflectDir = reflect(-direction, normal);
    vec3 viewDir = normalize(viewPos - vFragPos);

    float specularStrength = 0.3;
    float specularFactor = pow(max(dot(reflectDir, viewDir), 0.0), 512); 
    vec3 specular = specularStrength * specularFactor * lightColor;


    FragColor = vec4((ambient + diffuse + specular) * objectColor, 1.0);

}