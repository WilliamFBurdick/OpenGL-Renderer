#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	FragPos = vec3(model * vec4(aPos, 1.0));
	// Calculating the inverse is costly. So to be more efficient, the normal matrix
	// should instead be calculated on the CPU and passed to the GPU through a uniform
	Normal = mat3(transpose(inverse(model))) * aNormal;
	TexCoords = aTexCoords;
}