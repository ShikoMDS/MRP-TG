/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand

(c) 2024 Media Design School

File Name : Terrain.frag
Description : Fragment shader file for reflection model rendering (not implemented)
Author : Shikomisen (Ayoub Ahmad)
Mail : ayoub.ahmad@mds.ac.nz
**************************************************************************/

#version 460 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D grassTexture;
uniform sampler2D dirtTexture;
uniform sampler2D stoneTexture;
uniform sampler2D snowTexture;
uniform float height;

void main() {
    if (height < 0.2) {
        FragColor = texture(grassTexture, TexCoord);
    } else if (height < 0.4) {
        FragColor = mix(texture(grassTexture, TexCoord), texture(dirtTexture, TexCoord), (height - 0.2) * 5.0);
    } else if (height < 0.7) {
        FragColor = mix(texture(dirtTexture, TexCoord), texture(stoneTexture, TexCoord), (height - 0.4) * 3.33);
    } else {
        FragColor = mix(texture(stoneTexture, TexCoord), texture(snowTexture, TexCoord), (height - 0.7) * 3.33);
    }
}
