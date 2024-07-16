#version 330 core
struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Camera {
    vec3 position;
};

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor; 

uniform Material material;
uniform Light light_l1;
uniform Camera camera_c1;

void main() 
{
    FragColor = vec4(light_l1.ambient, 1.0);
}
