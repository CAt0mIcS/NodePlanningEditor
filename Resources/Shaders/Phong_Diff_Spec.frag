#version 450 core
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) in vec3 inNormal;
layout(location = 1) in vec3 inFragPos;
layout(location = 2) in vec2 inUV;
layout(location = 3) in vec3 inViewPos;

layout(location = 0) out vec4 outColor;

float lightAmbient = 0.1f;
float lightDiffuse = 5.0f;
float lightSpecular = 1.0f;
vec3 lightPosition = vec3(0.0f, 0.0f, 0.0f);
vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);

layout(set = 1, binding = 2) uniform sampler2D samplerDiffuse;
layout(set = 1, binding = 3) uniform sampler2D samplerSpecular;
float materialShininess = 1.0f;
vec3 samplerSpecularColor = vec3(1.0f, 1.0f, 1.0f);

float specularStrength = 0.5f;


void main()
{
    // ambient
    vec3 ambient = lightAmbient * texture(samplerDiffuse, inUV).rgb;

    // diffuse (map)
    vec3 norm = normalize(inNormal);
    vec3 lightDir = normalize(lightPosition - inFragPos);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = lightDiffuse * diff * texture(samplerDiffuse, inUV).rgb * lightColor;

    // specular (map)
    vec3 viewDir = normalize(inViewPos - inFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), materialShininess);
    vec3 specular = lightSpecular * spec * texture(samplerSpecular, inUV).rgb;

    outColor = vec4(ambient + diffuse + specular, 1.0f);
}
