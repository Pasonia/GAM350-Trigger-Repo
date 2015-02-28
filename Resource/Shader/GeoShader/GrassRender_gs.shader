#version 420                                                                        
                                                                                    
uniform mat4 proj;
uniform mat4 view;
uniform vec3 quad1, quad2;
uniform float offset;

layout(points) in;
layout(triangle_strip) out;
layout(max_vertices = 4) out;

in vec3 vColorPass[];
in float fSizePass[];
flat in int iTypePass[];

smooth out vec2 vTexCoord;

void main()
{
	if(iTypePass[0] != 0)
	{
		vec3 vPosOld = gl_in[0].gl_Position.xyz;
		float fSize = fSizePass[0];
		mat4 mVP = proj*view;
		  
		vec3 vPos = vPosOld+(-quad1-quad2)*fSize;
		vTexCoord = vec2(1.0, 1.0);
		gl_Position = mVP*vec4(vPos + vec3(offset,0,0), 1.0);
		EmitVertex();
		
		vPos = vPosOld+(-quad1+quad2)*fSize;
		vTexCoord = vec2(1.0, 0.0);
		gl_Position = mVP*vec4(vPos, 1.0);
		EmitVertex();
		
		vPos = vPosOld+(quad1-quad2)*fSize;
		vTexCoord = vec2(0.0, 1.0);
		gl_Position = mVP*vec4(vPos + vec3(offset,0,0), 1.0);
		EmitVertex();
		
		vPos = vPosOld+(quad1+quad2)*fSize;
		vTexCoord = vec2(0.0, 0.0);
		gl_Position = mVP*vec4(vPos, 1.0);
		EmitVertex();

		EndPrimitive();
	}
}