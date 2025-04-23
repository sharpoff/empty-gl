#version 460 core

struct Material
{
    sampler2D ambient;
    sampler2D diffuse;
    sampler2D specular;
};

struct Light
{
    vec3 pos;
    vec3 color;
};

in vec2 outUV;
in vec3 outNormal;
in vec3 fragPos;
in vec3 viewPos;

out vec4 fragColor;

uniform Material material;
uniform Light light;

void main()
{
    vec3 color;

    vec3 ambientTexture = texture(material.diffuse, outUV).rgb;
    vec3 diffuseTexture = texture(material.diffuse, outUV).rgb;
    vec3 specularTexture = texture(material.specular, outUV).rgb;

    vec3 unitNormal = normalize(outNormal);
    vec3 lightDir = normalize(light.pos - fragPos);
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, viewDir);

    vec3 ambient = ambientTexture * 0.1 * light.color;
    vec3 diffuse =  diffuseTexture * max(dot(unitNormal, lightDir), 0.3) * light.color;
    vec3 specular = specularTexture * pow(max(dot(viewDir, reflectDir), 0.0), 32) * 0.3 * light.color;
    // vec3 specular = vec3(0.0);

    color = ambient + diffuse + specular;
    fragColor = vec4(color, 1.0);
}
