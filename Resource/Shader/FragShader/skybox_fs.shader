#version 420 core

layout (binding = 0)uniform samplerCube texDay;
layout (binding = 1)uniform samplerCube texDawnDusk;
layout (binding = 2)uniform samplerCube texNight;

uniform float time;
uniform float state;

in VS_OUT
{
    vec3    tc;
} fs_in;

out vec4 color;

void main(void)
{
	vec4 DDtex = texture(texDawnDusk, fs_in.tc);
	vec4 DAYtex = texture(texDay, fs_in.tc);
	vec4 NIGHTtex = texture(texNight, fs_in.tc);

	//skybox texture output
	if(state == 0)//dawn
	{
		color = mix(DDtex, DAYtex, time);//texture(texDawnDusk, fs_in.tc);
	}
	if(state == 1)//day
	{
		color = mix(DAYtex, DDtex, time);//texture(texDay, fs_in.tc);
	}	
	if(state == 2)//dust
	{
		color = mix(DDtex, NIGHTtex, time);//texture(texDawnDusk, fs_in.tc);
	}	
	if(state == 3)//night
	{
		color = mix(NIGHTtex, DDtex, time);//texture(texNight, fs_in.tc);
	}
}
