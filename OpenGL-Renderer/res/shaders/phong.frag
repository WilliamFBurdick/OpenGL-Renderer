#version 460 core
#define MAX_POINT_LIGHTS 10
#define MAX_SPOT_LIGHTS 10
#define MAX_DIR_LIGHTS 10

out vec4 FragColor;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct DirLight{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight {
    vec3 position;  
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    float constant;
    float linear;
    float quadratic;
};

uniform int numPointLights;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform int numSpotLights;
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];
uniform int numDirLights;
uniform DirLight dirLights[MAX_DIR_LIGHTS];

uniform Material material;
// Most people calculate lighting in View Space. (ViewerPos would be 0,0,0)
// To do that, we'd apply View transform to the vectors and change the normal matrix
uniform vec3 viewPos;

in vec3 Normal;
in vec2 TexCoords;
in vec3 FragPos;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	// properties
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result = vec3(0.0);
	// Directional Shading
	for (int i = 0; i < numDirLights && i < MAX_DIR_LIGHTS; i++)
	{
		result += CalcDirLight(dirLights[i], norm, viewDir);
	}
	// Point Lights
	for (int i = 0; i < numPointLights && i < MAX_POINT_LIGHTS; i++)
	{
		result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
	}
	// Spot lights
	for (int i = 0; i < numSpotLights && i < MAX_SPOT_LIGHTS; i++)
	{
		result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir);
	}

	FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(viewDir, halfwayDir), 0.0), material.shininess);
	// combine results
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(viewDir, halfwayDir), 0.0), material.shininess);
	// attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	// combine results
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	if (theta > light.outerCutOff)
	{
		// diffuse shading
		float diff = max(dot(normal, lightDir), 0.0);
		// specular shading
		vec3 halfwayDir = normalize(lightDir + viewDir);
		float spec = pow(max(dot(viewDir, halfwayDir), 0.0), material.shininess);
		// attenuation
		float distance = length(light.position - fragPos);
		float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

		// combine results
		vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
		vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
		vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
		ambient *= attenuation;
		diffuse *= attenuation;
		specular *= attenuation;

		diffuse *= intensity;
		specular *= intensity;

		return (ambient + diffuse + specular);
	}
	else
	{
		return vec3(0.0);
	}
}

