#version 330                                                                        
                                                                                    
layout(points) in;
layout(points) out;
layout(max_vertices = 1000) out;

// All that we get from vertex shader

in vec3 vPositionPass[];
in vec3 vScalePass[];
flat in int iTypePass[];

// All that we send further

out vec3 PositionOut;
out float SizeOut;
flat out int TypeOut;

uniform vec3 Position; // Position where new particles are spawned
uniform float Scale;

void main()
{
	TypeOut = iTypePass[0];

	if(TypeOut == 0)
	{
		EmitVertex();
		EndPrimitive();
		
		for(int j = 0; j < 10; j++)
		{
			for(int i = 0; i < 10; i++)
			{
			  PositionOut = Position + vec3(i * 10, 0, j * 10);
			  SizeOut = Scale;
			  TypeOut = 1;
			  EmitVertex();
			  EndPrimitive();
			}
		}
	}
  else
  {
      EmitVertex();
      EndPrimitive(); 
  } 


}