#version 430

layout(location = 0) in vec3 normal;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 types;
layout(location = 3) in vec2 texCoord2;

layout(location = 0) uniform sampler2D grass;
layout(location = 1) uniform sampler2D snow;
layout(location = 2) uniform sampler2D rock;
layout(location = 3) uniform sampler2D mask;
uniform vec3 colour;

out vec4 fragColour;

void main()
{
    vec3 n = normalize(normal);
    vec3 lightDir = normalize(vec3(0.0, 0, 0.5));
    float diffuse = max(0.0, dot(n, -lightDir)) * 0.75;

    vec3 fogColour = vec3(0.0, 0.0, 0.5);
    float fogValue = (gl_FragCoord.z / gl_FragCoord.w) / 30.0;
    
    fragColour = vec4(fogColour * fogValue + vec3(diffuse) * (1.0 - fogValue), 1.0);
}
