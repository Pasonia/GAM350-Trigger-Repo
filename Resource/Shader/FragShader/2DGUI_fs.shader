#version 420 core

// Interpolated values from the vertex shaders
in vec2 UV;

uniform float alpha_value;
uniform bool isAlpha;
// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh.
uniform sampler2D ImageGUI2D;

void main()
{
	vec4 tex = texture2D( ImageGUI2D, UV);

	if(isAlpha)
		tex.a -= alpha_value;

	color = tex;
	
}