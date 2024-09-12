#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D diffuse;

void main()
{
	FragColor = vec4(texture(diffuse, TexCoords));
}