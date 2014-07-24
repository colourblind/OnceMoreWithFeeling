#version 410

layout(location = 0) in vec3 normal;
layout(location = 1) in vec3 position;

layout(location = 0) out vec4 fragColour;

uniform vec3 colour;
uniform vec3 eyePosition;
uniform float shininess;
uniform float reflectiveness;

uniform samplerCube environment;

void main()
{
    vec3 n = normalize(normal);
    vec3 lightDir = normalize(vec3(4.0, 1.0, 4.0));
    
    vec3 diffuse = max(0.0, dot(n, lightDir)) * colour;
    vec3 reflected = texture(environment, n).rgb * colour;
    
    vec3 lightReflect = reflect(-lightDir, n);
    vec3 eyeVector = normalize(eyePosition - position);
    float s = dot(lightReflect, eyeVector);
    vec3 specular = vec3(0.5 * pow(max(0.0, s), shininess));
    
    fragColour = vec4((1.0 - reflectiveness) * diffuse + reflectiveness * reflected + specular, 1.0);
}
