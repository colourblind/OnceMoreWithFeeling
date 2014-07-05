#version 400

in vec3 lightIntensity;

layout(location = 0) out vec4 fragColour;

uniform vec3 colour;

void main()
{
    fragColour = vec4(colour * lightIntensity, 1.0);
}
