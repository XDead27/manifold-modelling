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

void main()
{
	vec4 euclidean_pos = model * vec4(vPos, 1.0);
	float lorentz_factor = sqrt(ball_radius * ball_radius + dot(euclidean_pos.xyz, euclidean_pos.xyz));
	vec3 klein_pos = (ball_radius / lorentz_factor) * euclidean_pos.xyz;
	gl_Position = projection * view * vec4(klein_pos.xyz, 1.0f);
	TexCoords = vTexCoords;
	Normal = vNormal;
	FragPos = vec3(model * vec4(vPos, 1.0)); // not working as intended
}
