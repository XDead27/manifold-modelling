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
    vec3 lightDir = normalize(light_l1.position - FragPos);
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 viewDir = normalize(camera_c1.position - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient  = light_l1.ambient  * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 diffuse  = light_l1.diffuse  * diff * vec3(texture(material.texture_diffuse1, TexCoords));  
    vec3 specular = light_l1.specular * spec * vec3(texture(material.texture_specular1, TexCoords));
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}
