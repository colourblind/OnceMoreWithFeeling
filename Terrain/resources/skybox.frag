#version 430

layout(location = 0) in vec3 texCoord;

layout(location = 0) out vec4 fragColour;

layout(location = 0) uniform samplerCube environment;

void main()
{
    //fragColour = texture(environment, normalize(texCoord));
    float x = clamp(0.0, 1.0, dot(vec3(0.0, 1.0, 0.0), normalize(texCoord)) * 1.5);
    vec3 skyColour = smoothstep(vec3(0.0, 0.0, 1.0), vec3(1.0, 1.0, 0.5), vec3(x));
    fragColour = vec4(skyColour, 1.0);
}
