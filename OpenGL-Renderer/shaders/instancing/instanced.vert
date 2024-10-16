#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoords;
// mat4 variable type is larger than a vec4, so stored differently
// mat4 = 4 * vec4. So locations are 3, 4, 5, and 6.
layout (location = 3) in mat4 instanceMatrix;

out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main(void)
{
	gl_Position = projection * view * instanceMatrix * vec4(aPos, 1.0);
	TexCoords = aTexCoords;
}