#version 410

layout(location = 0) in vec3 normal;
layout(location = 1) in vec3 worldspacePosition;

layout(location = 0) out vec4 fragColour;

uniform vec3 colour;
uniform float specular;
uniform float shininess;
uniform float reflectiveness;

uniform samplerCube environment;

void main()
{
    vec3 n = normalize(normal);
    
    vec3 lightPosition = vec3(4.0, 0.0, 4.0);
    float attenuation = 1.0 / length(worldspacePosition - lightPosition);
    vec3 lightDir = normalize(worldspacePosition - lightPosition);
    
    float diffuse = attenuation * max(0.0, dot(n, -lightDir));
    
    fragColour = vec4((0.1 + 0.9 * diffuse) * colour, 1.0);
}
