#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoords;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

float ball_radius = 1.0f;

#define K -1 
#define tan_k tanh

vec3 qtransform(vec4 q, vec3 v)
{
	vec3 t = 2 * cross(q.xyz, v);
	return v + q.w * t + cross(q.xyz, t);
}

vec3 mobius_add(vec3 b, vec3 a, inout vec3 n) {
	vec3 c = K * cross(a , b);
	float d = 1.0f - K * dot(a, b);
	vec3 t = a + b;
	n = qtransform(normalize(vec4(c, d)), n);
	return (t * d + cross(c, t)) / (d * d + dot(c, c));
}

void main()
{

}
