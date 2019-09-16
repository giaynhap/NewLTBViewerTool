#pragma once
#include "math.h"
# define M_PIl          3.141592653589793238462643383279502884L 

typedef float vec3_t[3];
typedef float vec2_t[2];
typedef float vec4_t[4];
typedef vec4_t matt4x4_t[4];

typedef unsigned char  byte; 
typedef unsigned short  word;
typedef unsigned long  DWORD; 

float DotProduct(vec3_t x, vec3_t y);
bool InverseMat(matt4x4_t m, matt4x4_t& invOut);
void VectorTransform(vec3_t in1, matt4x4_t in2, vec3_t& out);
void VectorRotate(vec3_t in1, matt4x4_t in2, vec3_t& out);
void QuaternionMatrix(vec4_t quaternion, matt4x4_t& matrix);
void PostionSlerp(vec3_t p, vec3_t  p2, float t, vec3_t& presual);
void QuaternionSlerp(vec4_t p, vec4_t q, float t, vec4_t& qt);
void ConcatTransforms(matt4x4_t in1, matt4x4_t in2, matt4x4_t& out);