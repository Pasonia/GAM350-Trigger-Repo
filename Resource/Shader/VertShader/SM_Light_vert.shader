#version 420 core

uniform mat4 Lproj;
uniform mat4 Lview;
uniform mat4 model;

layout (location = 0) in vec4 pos;
layout (location = 1) in vec4 norm;
layout (location = 2) in vec2 tc;
layout (location = 3) in ivec4 BoneIDs;
layout (location = 4) in vec4 Weights;


void main(void)
{
    gl_Position = Lproj * Lview * model * pos;
}

