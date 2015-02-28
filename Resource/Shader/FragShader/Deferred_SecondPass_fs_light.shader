#version 420 core

#define NUM_LIGHTS 32
uniform int TOTAL_LIGHT_;
uniform vec3 lightPos[NUM_LIGHTS];
// Material properties
uniform vec3 diffuse_albedo[NUM_LIGHTS];
uniform vec3 specular_albedo[NUM_LIGHTS];
uniform vec3 ambient;
uniform vec3 LightColor[NUM_LIGHTS];
uniform float LightIntensity[NUM_LIGHTS];

vec3 SunLight(vec3 N, vec3 V)
{
	vec3 L = normalize(lightPos[0]);
		
	float cosTheta = clamp(dot(N,L), 0.0, 1.0);
	vec3 R = reflect(-L,N);
	float cosAlpha = clamp(dot(V,R), 0.0, 1.0);

	return (diffuse_albedo[0]  * LightColor[0] * LightIntensity[0] * cosTheta) +
		   (specular_albedo[0] * LightColor[0] * LightIntensity[0] * pow(cosAlpha, 4));	
}

vec3 ComputeLight(vec3 fpos, vec3 N, vec3 V)
{
	//main light source
	vec3 Final_Light = SunLight(N,V);

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


