#version 420 core

layout (location = 0) in vec4 pos;
layout (location = 1) in vec4 norm;
layout (location = 2) in vec2 tc;
layout (location = 3) in ivec4 BoneIDs;
layout (location = 4) in vec4 Weights;

out VS_OUT
{
    vec3 tc;
} vs_out;

uniform mat4 modelMTX;
uniform mat4 viewMTX;
uniform mat4 projMTX;

void main(void)
{
	//the ccw quad
 	//model view matrix
	mat4 mv_matrix = viewMTX * modelMTX;
	//set postion
    vec4 wvp = projMTX * mv_matrix * pos;
	gl_Position = wvp.xyww;

	vs_out.tc = pos.xyz;
}