#version 430

layout(location = 0) in vec3 viewspacePosition;

layout(location = 0) out vec4 fragColour;

layout(location = 0) uniform samplerCube environment;

void main()
{
    vec3 vp = normalize(viewspacePosition);
    fragColour = texture(environment, vp);
}
