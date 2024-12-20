#version 430 core
out float FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D texNoise;

uniform vec3 samples[64];

// parameters
uniform int kernelSize;
uniform float radius;
uniform float bias;

// tile noise texture over screen based on screen dimensions divided by noise size
uniform float screen_width;
uniform float screen_height;

uniform mat4 projection;

void main(void)
{
    vec2 noiseScale = vec2(screen_width / 4.0, screen_height / 4.0);
    // get input for SSAO algorithm
    vec3 fragPos = texture(gPosition, TexCoords).rgb;
    vec3 normal = normalize(texture(gNormal, TexCoords).rgb);
    vec3 randomVec = normalize(texture(texNoise, TexCoords * noiseScale).xyz);
    // create TBN change-of-basis matrix: from tangent space to view space
    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);
    // iterate over the sample kernel and calculate occlusion factor
    float occlusion = 0.0;
    for (int i = 0; i < kernelSize; i++)
    {
        // get sample position
        vec3 samplePos = TBN * samples[i];  // from tangent to view-space
        samplePos = fragPos + samplePos * radius;

        // project sample position (to sample texture) (to get position on screen/texture)
        vec4 offset = vec4(samplePos, 1.0);
        offset = projection * offset;   // from view to clip-space
        offset.xyz /= offset.w;         // perspective divide
        offset.xyz = offset.xyz * 0.5 + 0.5;    // clamp within range 0.0 - 1.0

        // get sample depth
        float sampleDepth = texture(gPosition, offset.xy).z;    // get depth value of kernel sample

        // range check & accumulate
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
        occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0) * rangeCheck;
    }
    occlusion = 1.0 - (occlusion / kernelSize);

    FragColor = occlusion;
}