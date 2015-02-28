#version 420 core

layout (location = 0) in vec4 pos;
layout (location = 1) in vec4 norm;
layout (location = 2) in vec2 tc;
layout (location = 3) in ivec4 BoneIDs;
layout (location = 4) in vec4 Weights;

#define MAX_BONES 64

uniform mat4 viewMat;
uniform mat4 modelMat;
uniform mat4 projMat;
uniform mat4 gBones[MAX_BONES];

//is to draw animation or static object
uniform bool isANIMATION;


out VS_OUT
{
    vec3        position;
    vec3        normal;
    vec2        tc;
} vs_out;

void main(void)
{
	//model view matrix
	mat4 mv_matrix = viewMat * modelMat;
	vec4 PosL;
	vec4 normalL;
	if(isANIMATION)
	{
     mat4 BoneTransform   = gBones[BoneIDs[0]] * Weights[0];
          BoneTransform  += gBones[BoneIDs[1]] * Weights[1];
          BoneTransform  += gBones[BoneIDs[2]] * Weights[2];
          BoneTransform  += gBones[BoneIDs[3]] * Weights[3];
		 
	   PosL = BoneTransform * pos;
	   normalL = BoneTransform * norm;
	}
    else
	{
		PosL = pos;
		normalL = norm;
	}
	//set postion
    gl_Position = projMat * mv_matrix * PosL;

	//out data
    vs_out.position =(modelMat * PosL).xyz;
    vs_out.normal = (transpose(inverse(modelMat)) * normalL).xyz;
    vs_out.tc = tc;
}