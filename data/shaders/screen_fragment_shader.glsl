#version 330 core

// Input texture coordinate from vertex shader
in vec2 TexCoords;

// Output color
out vec4 FragColor;

// The texture sampler
uniform sampler2D screenTexture;

void main()
{
	// Sample the texture color
	vec4 color = texture(screenTexture, TexCoords);
	// Output the final color
	FragColor = color;
}
