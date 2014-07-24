#version 410

layout(location = 0) in vec3 viewspacePosition;

layout(location = 0) out vec4 fragColour;

uniform mat4 v;

uniform vec3 colour;
uniform vec3 eyePosition;
uniform float specular;
uniform float shininess;
uniform float reflectiveness;

uniform samplerCube environment;

void main()
{
    vec3 vp = normalize(viewspacePosition);
    fragColour = texture(environment, vp);
}
