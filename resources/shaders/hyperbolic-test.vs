#version 410 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoords;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 hyperRot;

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
	vec4 w_pos = model * vec4(vPos, 1.0);
	Normal = vNormal; // i am not sure about this

	float w_dot = 0.0f;
	// Extract translation from view matrix (maybe not needed)
	mat4 matV = view;
	matV[0][3] = 0.0;
	matV[1][3] = 0.0;
	matV[2][3] = 0.0;
	vec3 vrPos = (vec4(view[0][3], view[1][3], view[2][3], 0.0) * matV).xyz;

	vec3 hyperPos = vec3(hyperRot[0][3], hyperRot[1][3], hyperRot[2][3]);
	w_pos.xyz = mobius_add(w_pos.xyz, hyperPos, Normal);
	w_pos.xyz = (hyperRot * vec4(w_pos.xyz, 0.0)).xyz;
	Normal = (hyperRot * vec4(Normal, 0.0)).xyz;
	w_pos.xyz = mobius_add(w_pos.xyz, vrPos, Normal);
	w_dot = dot(w_pos.xyz, w_pos.xyz);

	// Project to Beltrami-Klein
	w_pos.w *= 1.0 + w_dot;

	// gl_Position = projection * view * w_pos;
	gl_Position = projection * view * w_pos;
	TexCoords = vTexCoords;
	FragPos = w_pos.xyz;
}
