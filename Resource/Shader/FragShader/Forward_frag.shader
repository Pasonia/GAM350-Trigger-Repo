#version 420 core

layout (binding = 0) uniform sampler2D textureColor;
layout (binding = 1) uniform sampler2D textureColor1;
layout (binding = 2) uniform sampler2D textureColor2;
layout (binding = 3) uniform sampler2D textureColor3;
layout (binding = 4) uniform sampler2D shadow_tex;

in VS_OUT
{
    vec3        position;
    vec3        normal;
    vec2        tc;
	vec4 		shadow_coord;
} fs_in;

uniform bool draw_type;

//scale in texture size
uniform vec2 Tex_scale;
uniform vec3 CamPos;
#define NUM_LIGHTS 32
uniform int TOTAL_LIGHT_;
uniform vec3 lightPos[NUM_LIGHTS];
// Material properties
uniform vec3 diffuse_albedo[NUM_LIGHTS];
uniform vec3 specular_albedo[NUM_LIGHTS];
uniform vec3 ambient;
uniform vec3 LightColor[NUM_LIGHTS];
uniform float LightIntensity[NUM_LIGHTS];
uniform vec3 characPOS;
out vec4 FragColor;


vec2 poissonDisk[16] = vec2[]( 
   vec2( -0.94201624, -0.39906216 ), 
   vec2( 0.94558609, -0.76890725 ), 
   vec2( -0.094184101, -0.92938870 ), 
   vec2( 0.34495938, 0.29387760 ), 
   vec2( -0.91588581, 0.45771432 ), 
   vec2( -0.81544232, -0.87912464 ), 
   vec2( -0.38277543, 0.27676845 ), 
   vec2( 0.97484398, 0.75648379 ), 
   vec2( 0.44323325, -0.97511554 ), 
   vec2( 0.53742981, -0.47373420 ), 
   vec2( -0.26496911, -0.41893023 ), 
   vec2( 0.79197514, 0.19090188 ), 
   vec2( -0.24188840, 0.99706507 ), 
   vec2( -0.81409955, 0.91437590 ), 
   vec2( 0.19984126, 0.78641367 ), 
   vec2( 0.14383161, -0.14100790 ) 
);

vec3 SunLight1(vec3 N, vec3 V)
{
	vec3 L = normalize(lightPos[0]);
		
	float cosTheta = clamp(dot(N,L), 0.0, 1.0);
	vec3 R = reflect(-L,N);
	float cosAlpha = clamp(dot(V,R), 0.0, 1.0);

	return (diffuse_albedo[0]  * LightColor[0] * LightIntensity[0] * cosTheta) +
		   (specular_albedo[0] * LightColor[0] * LightIntensity[0] * pow(cosAlpha, 4));	
}

vec3 ComputeLight1(vec3 fpos, vec3 N, vec3 V)
{
	//main light source
	vec3 Final_Light = SunLight1(N,V);
	float visibility=1.0;
	float bias = 0.005;
	for(int i = 0; i < 4; i++)
		{
			// use either :
			//  - Always the same samples.
			//    Gives a fixed pattern in the shadow, but no noise
			int index = i;
			//  - A random sample, based on the pixel's screen location. 
			//    No banding, but the shadow moves with the camera, which looks weird.
			// int index = int(16.0*random(gl_FragCoord.xyy, i))%16;
			//  - A random sample, based on the pixel's position in world space.
			//    The position is rounded to the millimeter to avoid too much aliasing
			// int index = int(16.0*random(floor(Position_worldspace.xyz*1000.0), i))%16;
		
			// being fully in the shadow will eat up 4*0.2 = 0.8
			// 0.2 potentially remain, which is quite dark.
			//visibility -= 0.2*(1.0 - texture(shadow_tex, vec3(fs_in.shadow_coord.xy/fs_in.shadow_coord.w + poissonDisk[index]/700.0, (fs_in.shadow_coord.z-bias)/fs_in.shadow_coord.w) ));
		}
	for(int i = 1; i < TOTAL_LIGHT_; ++i)
	{
		vec3 L = normalize(lightPos[i] - fpos);
		float distance = length(lightPos[i] - fpos);
		
		float cosTheta = clamp(dot(N,L), 0.0, 1.0);
		vec3 R = reflect(-L,N);
		float cosAlpha = clamp(dot(V,R), 0.0, 1.0);

		Final_Light += (diffuse_albedo[i]  * LightColor[i] * LightIntensity[i] * cosTheta /(distance) ) +
					   (specular_albedo[i] * LightColor[i] * LightIntensity[i] * pow(cosAlpha, 4) / (distance));	
	}

	return Final_Light += ambient;
}

void main()
{
	vec2 textureCoord = vec2(fs_in.tc.x * Tex_scale.x, fs_in.tc.y * Tex_scale.y);

	vec3 N = normalize(fs_in.normal);
	vec3 V = (CamPos - fs_in.position);
	vec3 sunlight = ComputeLight1(fs_in.position,N,V);
	
	if(draw_type)
	{
		vec4 texValue  = texture2D(textureColor, textureCoord);
		vec4 texValue1 = texture2D(textureColor1, textureCoord);
		vec4 texValue2 = texture2D(textureColor2, textureCoord);
		vec4 texValue3 = texture2D(textureColor3, textureCoord);
		
		float slope = 1.0f - N.y;
		vec4 texCOL;
		float blendAmount;
		// Determine which texture to use based on height.
		if(slope < 0.1)
		{
			blendAmount = slope / 0.1f;
			texCOL = mix(texValue, texValue1, blendAmount);
		}	
		if((slope < 0.5) && (slope >= 0.1f))
		{
			blendAmount = (slope - 0.1f) * (1.0f / (0.5f - 0.1f));
			texCOL = mix(texValue1, texValue2, blendAmount);
		}
		if(slope >= 0.5) 
		{
			texCOL = texValue3;
		}
		FragColor =	vec4(sunlight * texCOL.rgb,0);
	}
	else
	{	
		vec4 tex = texture2D(textureColor, textureCoord);
		if(tex.a < 0.5)
			discard;
		FragColor =	vec4(sunlight * tex.rgb,tex.a);
	}	
	//if(texture(shadow_tex,fs_in.shadow_coord.xyz/fs_in.shadow_coord.w) <= 0.f )
		//		float d = texelFetch(shadow_tex, ivec2(gl_FragCoord.xy), 0).r;
		//			d = (d - 0.95) * 15.0;
			//		FragColor = vec4(d);
	//else
	//	FragColor = vec4(0,0,0,0.0f);
}

