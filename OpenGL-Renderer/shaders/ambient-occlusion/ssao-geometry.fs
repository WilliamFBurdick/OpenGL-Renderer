#version 430 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedo;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

void main(void)
{
    // store fragment position in first gbuffer texture
    gPosition = FragPos;
    // store per-fragment normals in second gbuffer texture
    gNormal = normalize(Normal);
    // store diffuse per-fragment color
    gAlbedo.rgb = vec3(0.95);
}