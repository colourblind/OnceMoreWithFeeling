#version 430

layout(location = 0) in vec3 texCoord;

layout(location = 0) out vec4 fragColour;

layout(location = 0) uniform samplerCube environment;

void main()
{
    fragColour = texture(environment, normalize(texCoord));
}
