#version 430

layout(location = 0) in float diffuse;

layout(location = 0) out vec4 fragColour;
layout(location = 1) out vec4 glowColour;

uniform vec3 colour;

void main()
{
    fragColour = vec4(colour * (0.25 + diffuse * 0.75), 1.0);
    glowColour = vec4(1.0 - diffuse, 1.0 - diffuse, 1.0 - diffuse, 1.0);
}
