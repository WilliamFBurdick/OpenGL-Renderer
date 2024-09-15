#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D diffuse;

uniform int enabledEffect;

void main()
{
	vec3 col = texture(diffuse, TexCoords).rgb;
	FragColor = vec4(col, 1.0);

	const float offset = 1.0 / 300.0;
	vec2 offsets[9] = vec2[](
		vec2(-offset, offset),
		vec2(0.0, offset),
		vec2(offset, offset),
		vec2(-offset, 0.0),
		vec2(0.0, 0.0),
		vec2(offset, 0.0),
		vec2(-offset, -offset),
		vec2(0.0, -offset),
		vec2(offset, -offset)
	);

	// Invert
	if (enabledEffect == 1)
	{
		FragColor = vec4(vec3(1.0 - texture(diffuse, TexCoords)), 1.0);
	}
	// Greyscale
	if (enabledEffect == 2)
	{
		//FragColor = texture(diffuse, TexCoords);
		float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
		FragColor = vec4(average, average, average, 1.0);
	}
	// Sharpen
	if (enabledEffect == 3)
	{
		float kernel[9] = float[](
			-1, -1, -1,
			-1, 9, -1,
			-1, -1, -1
		);
		vec3 sampleTex[9];
		for (int i = 0; i < 9; i++)
			sampleTex[i] = vec3(texture(diffuse, TexCoords.st + offsets[i]));
		vec3 col = vec3(0.0);
		for (int i = 0; i < 9; i++)
			col += sampleTex[i] * kernel[i];

		FragColor = vec4(col, 1.0);
	}
	// Blur
	if (enabledEffect == 4)
	{
		float kernel[9] = float[](
			1.0 / 16, 2.0 / 16, 1.0 / 16,
			2.0 / 16, 4.0 / 16, 2.0 / 16,
			1.0 / 16, 2.0 / 16, 1.0 / 16
		);
		vec3 sampleTex[9];
		for (int i = 0; i < 9; i++)
			sampleTex[i] = vec3(texture(diffuse, TexCoords.st + offsets[i]));
		vec3 col = vec3(0.0);
		for (int i = 0; i < 9; i++)
			col += sampleTex[i] * kernel[i];

		FragColor = vec4(col, 1.0);
	}
	// Edge detect
	if (enabledEffect == 5)
	{
		float kernel[9] = float[](
			1, 1, 1,
			1, -8, 1,
			1, 1, 1
		);
		vec3 sampleTex[9];
		for (int i = 0; i < 9; i++)
			sampleTex[i] = vec3(texture(diffuse, TexCoords.st + offsets[i]));
		vec3 col = vec3(0.0);
		for (int i = 0; i < 9; i++)
			col += sampleTex[i] * kernel[i];

		FragColor = vec4(col, 1.0);
	}
}