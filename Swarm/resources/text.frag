#version 410

layout(location = 0) in vec2 texCoord;

layout(location = 0) out vec4 fragColour;

uniform vec3 colour;
uniform sampler2D font;

void main()
{
    fragColour = vec4(colour, texture(font, texCoord).r);
}
