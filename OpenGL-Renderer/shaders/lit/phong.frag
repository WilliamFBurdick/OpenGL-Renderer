#version 430 core
out vec4 FragColor;

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Attenuation
{
	float constant, linear, quadratic;
};

struct DirectionalLight
{
	vec3 direction;
	vec3 ambient, diffuse, specular;
};

struct PointLight
{
	vec3 position;
	vec3 ambient, diffuse, specular;
	Attenuation attenuation;
};

struct SpotLight
{
	vec3 position;
	vec3 direction;
	vec3 ambient, diffuse, specular;
	Attenuation attenuation;
	float cutOff;
	float outerCutOff;
};


in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

// LIGHT SETTINGS

#define MAX_DIRECTIONAL_LIGHTS 1
#define MAX_POINT_LIGHTS 10
#define MAX_SPOT_LIGHTS 5

uniform vec3 viewPos;
uniform DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHTS];
uniform int numDirectionalLights;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform int numPointLights;
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];
uniform int numSpotLights;

// helper functions
vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main(void)
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result;
	for (int i = 0; i < numDirectionalLights && i < MAX_DIRECTIONAL_LIGHTS; i++)
		result += CalcDirLight(directionalLights[i], norm, viewDir);
	for (int i = 0; i < numPointLights && i < MAX_POINT_LIGHTS; i++)
		result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
	for (int i = 0; i < numSpotLights && i < MAX_SPOT_LIGHTS; i++)
		result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir);

	FragColor = vec4(result, 1.0);
}

