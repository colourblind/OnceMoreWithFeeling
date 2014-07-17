#version 410

layout(location = 0) in vec3 normal;
layout(location = 1) in vec3 position;

layout(location = 0) out vec4 fragColour;

uniform vec3 colour;
uniform vec3 eyePosition;
uniform float shininess;

void main()
{
    vec3 lightPosition = vec3(4.0, 1.0, 4.0);
    vec3 n = normalize(normal);
    vec3 lightDir = normalize(lightPosition - position);
    
    float diffuse = max(0.0, dot(n, lightDir));
    
    vec3 lightReflect = reflect(-lightDir, n);
    vec3 eyeVector = normalize(eyePosition - position);
    float specular = dot(lightReflect, eyeVector);
    specular = 0.5 * pow(max(0.0, specular), shininess);
    
    fragColour = vec4(colour * (diffuse + specular), 1.0);
}
