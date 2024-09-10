#version 460 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightDir = normalize(vec3(-0.2f, -1.0f, -0.3f)); // Direction of light
uniform vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);  // Light color
uniform vec3 objectColor = vec3(0.6f, 0.6f, 0.6f); // Default object color

void main() {
    // Ambient lighting
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse lighting
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Final color calculation
    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0f);
}
