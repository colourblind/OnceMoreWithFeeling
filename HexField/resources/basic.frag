#version 430

layout(location = 0) out vec4 fragColour;

uniform vec3 colour;
layout(location = 0) uniform float age;

void main()
{
    fragColour = vec4(colour, sqrt(sin(age)));
}
