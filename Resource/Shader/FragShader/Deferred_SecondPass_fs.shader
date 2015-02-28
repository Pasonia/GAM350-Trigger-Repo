#version 420 core

vec3 ComputeLight(vec3 fpos, vec3 N, vec3 V);

layout (location = 0) out vec4 fragColor;

layout (binding = 0) uniform sampler2D gdiffuse;
layout (binding = 1) uniform sampler2D gposition;
layout (binding = 2) uniform sampler2D gnormal;

#define FLT_EPSILON 1.19209290E-07F // decimal constant

//camera pos
uniform vec3 CamPos;
uniform bool Light_ON_OFF;

uniform bool isMotionBlur;

//window screen size
uniform vec2 WINDOWSIZE;

const float sampleDist = 512.0;
const float sampleStrength = 2.0;
// some sample positions
float samples[10] = float[](-0.08,-0.05,-0.03,-0.02,-0.01,0.01,0.02,0.03,0.05,0.08);

struct fragment_info
{
    vec3        position;
    vec3        normal;
    vec3        color;
};

vec3 RadBlurr(vec4 color)
{
	vec2 uv = vec2(gl_FragCoord.x/WINDOWSIZE.x,gl_FragCoord.y/WINDOWSIZE.y);
	vec2 dir = 0.5 - uv;
	// calculate the distance to the center of the screen
	float dist = sqrt(dir.x * dir.x + dir.y * dir.y); 
	dir = dir/dist; 
	vec4 sum = color;
	// take 10 additional blur samples in the direction towards
	// the center of the screen
	for (int i = 0; i < 8; i++)
	{
	float x = clamp(uv.x + dir.x * samples[i] * sampleDist, 0.0, WINDOWSIZE.x);
	float y = clamp(uv.y + dir.y * samples[i] * sampleDist, 0.0, WINDOWSIZE.y);
		sum += texture2D(gdiffuse, vec2(x,y));
	}
	sum *= 1.0/9.0;
	float t = dist * sampleStrength;
	t = clamp(t ,0.0f,1.0f);
	
	vec2 velocity = vec2(1,1) ;  
	vec2 texCoord = gl_FragCoord.xy;
	vec4 col_ = texelFetch(gdiffuse, ivec2(texCoord), 0);
	texCoord += velocity;

	for(int i = 1; i < 8; ++i, texCoord += velocity)  
	{  
		// Sample the color buffer along the velocity vector.  
		vec4 currentColor = texelFetch(gdiffuse, ivec2(texCoord), 0);  
		// Add the current color to our color sum.  
		col_ += currentColor;  
	}  
	col_ /= 16;

	return  mix(color, sum, t).rgb * col_.rgb * 4.0;
}

void Extract(vec2 coord, out fragment_info frag)
{
	frag.color = texture(gdiffuse, coord).rgb;
	frag.position = texture(gposition, coord).xyz;
	frag.normal = texture(gnormal, coord).rgb;
}

void main(void)
{
	fragment_info fragment;
	Extract(vec2(gl_FragCoord.x/ WINDOWSIZE.x, gl_FragCoord.y/WINDOWSIZE.y), fragment);

	if(Light_ON_OFF)
	{
		vec3 N = fragment.normal;
		N = normalize(N);
		vec3 fpos = fragment.position;
		vec3 V = normalize(CamPos - fpos);
		vec3 FinalLight = ComputeLight(fpos,N,V);

		vec3 final_color = fragment.color;
			
		if(isMotionBlur)
			final_color = RadBlurr(vec4(fragment.color,1));

		fragColor = vec4(final_color.rgb * FinalLight.rgb,0);
	}
	else
	{
		fragColor = vec4(fragment.color,0);
	}
	
}