#version 430

layout(location = 0) in vec3 colour;
layout(location = 1) in vec3 worldPosition;
layout(location = 2) in vec2 texCoord;

layout(location = 0) out vec4 fragColour;

layout(location = 1) uniform float lightStrength;
layout(location = 2) uniform vec3 lightPosition;
layout(location = 3) uniform vec3 normal;
layout(location = 4) uniform sampler2D cloudTexture;

void main()
{
    vec3 lightVector = (lightPosition - worldPosition);
    float lightDistance = length(worldPosition - lightPosition) * 0.15;
    float flash = dot(normal, -normalize(lightVector)) * lightStrength / pow(lightDistance, 2.0);
    
    float depth = gl_FragCoord.z;
    
    fragColour = vec4(colour * min(1.0, texCoord.t + 0.5) + max(0.0, flash), texture(cloudTexture, texCoord).r * pow(depth, 8.0));
}
