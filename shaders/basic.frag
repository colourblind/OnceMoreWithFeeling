#version 410

layout(location = 0) in vec3 normal;
layout(location = 1) in vec3 position;

layout(location = 0) out vec4 fragColour;

uniform vec3 colour;

void main()
{
    vec3 lightPosition = vec3(5.0, 0.5, -5.0);
    vec3 lightDir = normalize(position - lightPosition);
    float lightIntensity = max(0.0, dot(normal, lightDir));
    fragColour = vec4(colour * lightIntensity, 1.0);
}
