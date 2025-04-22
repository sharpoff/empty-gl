#version 460 core

struct Material
{
    sampler2D ambient;
    sampler2D diffuse;
    sampler2D specular;
};

in vec2 outUV;
in vec3 outNormal;

out vec4 fragColor;

uniform Material material;

void main()
{
    vec3 color;

    vec3 ambient = texture(material.ambient, outUV).rgb * 0.5;
    vec3 diffuse = texture(material.diffuse, outUV).rgb * 0.5;
    float specular = texture(material.specular, outUV).r * 0.1;
    color = ambient + diffuse + specular;

    fragColor = vec4(color, 1.0);
}
