#version 420 core

uniform sampler2D textureColor;

//uniform vec4 color;
//high color
uniform bool isHighLight;
//draw debug color
uniform bool isDebug;
//draw color
uniform bool isCollided;
//scale in texture size
uniform vec2 Tex_scale;

in VS_OUT
{
    vec3        position;
    vec3        normal;
    vec2        tc;
} fs_in;

layout (location = 0) out vec4 diffuse;
layout (location = 1) out vec4 position;
layout (location = 2) out vec4 normal;

void main(void)
{
	if(isDebug)
	{
		if(isCollided)
			diffuse = vec4(255.f,0,0,0);
		else
			diffuse = vec4(0,255.f,0,0);
	}
	else
	{
		vec2 textureCoord = vec2(fs_in.tc.x * Tex_scale.x, fs_in.tc.y * Tex_scale.y);
		vec4 texValue = texture2D(textureColor, textureCoord);
		if(isHighLight)
			diffuse  = vec4(texValue.r * 10.f,texValue.r * 0.5f,texValue.r * 0.5f,0);
		else
			diffuse  = texValue;
	}

	position = vec4(fs_in.position,1);
	normal   = vec4(fs_in.normal,0);
}
