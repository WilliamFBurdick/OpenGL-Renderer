#version 430 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

// which effect to apply
uniform int activeEffect;
// 0 = "none", 1 = "invert", 2 = "greyscale"
// Kernel Effects
// 3 = "sharpen", 4 = "blur", 5 = "edge detect"

const float offset = 1.0 / 300.0;

void main(void)
{
	vec2 offsets[9] = vec2[](
		vec2(-offset,	offset),	// top-left
		vec2(0.0,		offset),	// top-center
		vec2(offset,	offset),	// top-right
		vec2(-offset,	0.0),		// center-left
		vec2(0.0,		0.0),		// center-center
		vec2(offset,	0.0),		// center-right
		vec2(-offset,	-offset),	// bottom-left
		vec2(0.0,		-offset),	// bottom-center
		vec2(offset,	-offset)	// bottom-right
	);

	// No effect
	if (activeEffect == 0)
	{
		FragColor = texture(screenTexture, TexCoords);
	}
	// Invert
	else if (activeEffect == 1)
	{
		FragColor = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);
	}
	// Greyscale
	else if (activeEffect == 2)
	{
		FragColor = texture(screenTexture, TexCoords);
		float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
		FragColor = vec4(average, average, average, 1.0);
	}
	// Sharpen
	else if (activeEffect == 3)
	{
		float kernel[9] = float[](
			-1, -1, -1,
			-1, 9, -1,
			-1, -1, -1
		);
		vec3 sampleTex[9];
		for (int i = 0; i < 9; i++)
			sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
		vec3 col = vec3(0.0);
		for (int i = 0; i < 9; i++)
			col += sampleTex[i] * kernel[i];
		FragColor = vec4(col, 1.0);
	}
	// Blur
	else if (activeEffect == 4)
	{
		float kernel[9] = float[](
			1.0 / 16, 2.0 / 16, 1.0 / 16,
			2.0 / 16, 4.0 / 16, 2.0 / 16,
			1.0 / 16, 2.0 / 16, 1.0 / 16
		);
		vec3 sampleTex[9];
		for (int i = 0; i < 9; i++)
			sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
		vec3 col = vec3(0.0);
		for (int i = 0; i < 9; i++)
			col += sampleTex[i] * kernel[i];
		FragColor = vec4(col, 1.0);
	}
	// Edge detect
	else if (activeEffect == 5)
	{
		float kernel[9] = float[](
			1, 1, 1,
			1, -8, 1,
			1, 1, 1
		);
		vec3 sampleTex[9];
		for (int i = 0; i < 9; i++)
			sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
		vec3 col = vec3(0.0);
		for (int i = 0; i < 9; i++)
			col += sampleTex[i] * kernel[i];
		FragColor = vec4(col, 1.0);
	}
	else
	{
		FragColor = texture(screenTexture, TexCoords);
	}
}