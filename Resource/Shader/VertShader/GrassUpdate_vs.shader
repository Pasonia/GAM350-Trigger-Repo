#version 420                                                                        
                                                                                    
layout (location = 0) in vec3 vPosition;
layout (location = 1) in float vScale;
layout (location = 2) in int iType;

out vec3 vPositionPass;
out float vScalePass;
flat out int iTypePass;

void main()
{
  vPositionPass = vPosition;
  vScalePass = vScale;
  iTypePass = iType;
}