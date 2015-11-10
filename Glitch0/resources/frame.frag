#version 430

layout(location = 0) in vec2 texCoord;

layout(location = 0) uniform sampler2D frame;

out vec4 fragColour;

void main()
{
    fragColour = texture(frame, texCoord);
}
