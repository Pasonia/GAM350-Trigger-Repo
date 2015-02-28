#version 420 core

layout (location = 0) out float color;

void main(void)
{
    color = gl_FragCoord.z;
}
