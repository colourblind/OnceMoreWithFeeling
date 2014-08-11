#version 410

layout(location = 0) in vec3 colour;

layout(location = 0) out vec4 fragColour;

void main()
{
    fragColour = vec4(colour, 1.0);
}
