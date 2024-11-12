#version 430 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main(void)
{
    // store fragment position in first gbuffer texture
    gPosition = FragPos;
    // store fragment normal in second gbuffer texture
    gNormal = normalize(Normal);
    // store diffuse color
    gAlbedoSpec.rgb = texture(texture_diffuse1, TexCoords).rgb;
    // store specular intensity in alpha channel
    gAlbedoSpec.a = texture(texture_specular1, TexCoords).r;
}