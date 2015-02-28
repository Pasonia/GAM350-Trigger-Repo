#version 420                                                                        
                                                                                    
uniform sampler2D gSampler;

smooth in vec2 vTexCoord;
flat in vec4 vColorPart;

out vec4 FragColor;

void main()
{
  vec4 vTexColor = texture2D(gSampler, vTexCoord);
  FragColor = vec4(vTexColor.rgb, vTexColor.a);
}