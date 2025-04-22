#version 330 core

in vec2 texUV;
in vec3 normals;
in vec3 color;

out vec4 fragColor;

struct Material
{
    sampler2D ambient;
    sampler2D diffuse;
    sampler2D specular;
};

uniform Material material;

void main()
{
    vec3 color;
    vec3 ambient = texture(material.ambient, texUV).rgb * 0.5;
    vec3 diffuse = texture(material.diffuse, texUV).rgb * 0.5;
    float specular = texture(material.specular, texUV).r * 0.1;
    color = ambient + diffuse + specular;

    fragColor = vec4(color, 1.0);
}
