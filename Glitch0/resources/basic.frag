#version 430

layout(location = 0) in float diffuse;
layout(location = 1) in vec2 texCoord;

layout(location = 0) out vec4 fragColour;

uniform vec3 colour;

float rand(vec2 x)
{
    return fract(sin(dot(x.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{
    fragColour = vec4(colour * (0.25 + diffuse * 0.75), 1.0);
    float t = clamp(rand(texCoord), 0.0, 1.0);
    t = pow(t, 2.0) * 20.0 * (1.0 - gl_FragCoord.z);
    fragColour = fragColour * t;
    fragColour.a = t;
}
