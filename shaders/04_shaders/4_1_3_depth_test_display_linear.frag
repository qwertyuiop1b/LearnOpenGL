#version 330 core

in vec2 vTexcoords;
uniform sampler2D texture1;

float near = 0.1;
float far = 100.0;

float linearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}

out vec4 fragColor;

void main() {
    // fragColor = texture(texture1, vTexcoords);
    // fragColor = vec4(vec3(gl_FragCoord.z), 1.0);
    float depth = linearizeDepth(gl_FragCoord.z) / far;
    fragColor = vec4(vec3(depth), 1.0);

    // foggy effect
    // float depth = LinearizeDepth(gl_FragCoord.z) / far;
    // vec4 depthVec4 = vec4(vec3(pow(depth, 1.4)), 1.0);
    // FragColor = (ambient + diffuse + specular + emission) * (1 - depthVec4) + depthVec4;

    
}