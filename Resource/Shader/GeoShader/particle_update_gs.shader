#version 330

layout(points) in;
layout(points) out;
layout(max_vertices = 40) out;

// All that we get from vertex shader
in vec3 vPositionPass[];
in vec3 vVelocityPass[];
in vec3 vColorPass[];
in float fLifeTimePass[];
in float fSizePass[];
flat in int iTypePass[];

// All that we send further
out vec3 PositionOut;
out vec3 VelocityOut;
out vec3 ColorOut;
out float LifeTimeOut;
out float SizeOut;
flat out int TypeOut;

// vars from Particle.cpp
uniform float time; // Time passed since last frame
uniform vec3 Position; // Position where new particles are spawned
uniform vec3 VelocityMin; // Velocity of new particle - from min to (min+range)
uniform vec3 VelocityRange;
uniform vec3 Gravity; // Gravity vector for particles - updates velocity of particles
uniform float LifeTimeMin, LifeTimeRange; // Life of new particle - from min to (min+range)
uniform float Size;
uniform int NUMBER_GENERATE; // density, if greater than zero, particles are generated
uniform vec3 Color;
uniform vec3 vRandomSeed; // Seed number for our random number function
vec3 vLocalSeed;

// This function returns random number from zero to one
float randZeroOne()
{
    uint n = floatBitsToUint(vLocalSeed.y * 214013.0 + vLocalSeed.x * 2531011.0 + vLocalSeed.z * 141251.0);
    n = n * (n * n * 15731u + 789221u);
    n = (n >> 9u) | 0x3F800000u;

    float fRes =  2.0 - uintBitsToFloat(n);
    vLocalSeed = vec3(vLocalSeed.x + 147158.0 * fRes, vLocalSeed.y*fRes  + 415161.0 * fRes, vLocalSeed.z + 324154.0*fRes);
    return fRes;
}

void main()
{
  vLocalSeed = vRandomSeed;

  // gl_Position doesn't matter

  PositionOut = vPositionPass[0];
  VelocityOut = vVelocityPass[0];
  if(iTypePass[0] != 0)PositionOut += VelocityOut*time;
  if(iTypePass[0] != 0)VelocityOut += Gravity*time;

  ColorOut = vColorPass[0];
  LifeTimeOut = fLifeTimePass[0]-time;
  SizeOut = fSizePass[0];
  TypeOut = iTypePass[0];

  if(TypeOut == 0)
  {
    EmitVertex();
    EndPrimitive();

    for(int i = 0; i < NUMBER_GENERATE; i++)
    {
      PositionOut = Position + vec3(0.25*(randZeroOne()-randZeroOne()),0.25*(randZeroOne()-randZeroOne()),randZeroOne());
      VelocityOut = VelocityMin+vec3(VelocityRange.x*randZeroOne(), VelocityRange.y*randZeroOne(), VelocityRange.z*randZeroOne());
      ColorOut = Color;
      LifeTimeOut = LifeTimeMin+LifeTimeRange*randZeroOne();
      SizeOut = Size;
      TypeOut = 1;
      EmitVertex();
      EndPrimitive();
    }
  }
  else if(LifeTimeOut > 0.0)
  {
      EmitVertex();
      EndPrimitive();
  }
}