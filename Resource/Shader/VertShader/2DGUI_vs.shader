#version 420 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec2 vertexPosition_screenspace;
layout(location = 1) in vec2 vertexUV;

// Output data ; will be interpolated for each fragment.
out vec2 UV;
uniform vec2 WINDOWSIZE;

void main()
{
	vec2 half_size = vec2(WINDOWSIZE.x/2, WINDOWSIZE.y/2);
	// Output position of the vertex, in clip space
	vec2 vertexPosition_homoneneousspace = vertexPosition_screenspace - vec2(half_size.x,half_size.y);
	vertexPosition_homoneneousspace /= vec2(half_size.x,half_size.y);
	gl_Position =  vec4(vertexPosition_homoneneousspace,0,1);
	
	// UV of the vertex. No special space for this one.
	UV = vertexUV;
}
