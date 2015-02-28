#version 420                                                              

layout (location = 0) in vec4 pos;
layout (location = 1) in vec4 norm;
layout (location = 2) in vec2 tc;
layout (location = 3) in ivec4 BoneIDs;
layout (location = 4) in vec4 Weights;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;
uniform mat4 shadowMat;
uniform mat4 bias_scale;

out VS_OUT
{
    vec3        position;
    vec3        normal;
    vec2        tc;
	vec4 		shadow_coord;
} vs_out;

void main()
{
 	//model view matrix
	mat4 mv_matrix = viewMat * modelMat;
	//set postion
    gl_Position = projMat * mv_matrix * pos;
	//out data
    vs_out.position =(modelMat * pos).xyz;
    vs_out.normal = (transpose(inverse(modelMat)) * norm).xyz;
    vs_out.tc = tc;
	mat4 shadow_matrix = bias_scale * projMat * shadowMat * modelMat;
	vs_out.shadow_coord = shadow_matrix * pos;
}