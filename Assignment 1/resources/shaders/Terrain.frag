#version 460 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D terrainTexture;

void main() {
    vec3 color = texture(terrainTexture, TexCoords).rgb;
    FragColor = vec4(color, 1.0);
}
