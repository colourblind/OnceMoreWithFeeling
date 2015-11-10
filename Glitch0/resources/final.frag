#version 430

layout(location = 0) in vec2 texCoord;

layout(location = 0) uniform sampler2D colour;
layout(location = 1) uniform sampler2D depth;
layout(location = 2) uniform sampler2D noise0;
layout(location = 3) uniform sampler2D noise1;

layout(location = 5) uniform mat4 view;

layout(location = 0) out vec4 fragColour;
layout(location = 1) out vec4 saveColour;

void main()
{
    fragColour = texture(colour, texCoord);
    saveColour = fragColour;
}
