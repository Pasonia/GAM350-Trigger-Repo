#version 420                                                                        
                                                                                    
layout (location = 0) in vec3 vPosition;
layout (location = 1) in float vSize;
layout (location = 2) in int iType;

out float fSizePass;
flat out int iTypePass;

void main()
{
   gl_Position = vec4(vPosition, 1.0);
   fSizePass = vSize;
   iTypePass = iType;
}