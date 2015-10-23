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
    
    float depth = gl_FragCoord.z / gl_FragCoord.w;
    float alpha = sin(clamp(depth / 50.0, 0.0, 1.0) * 3.142);
    
    // rgb: passed colour * cloud.y (using texCoord.t, start falloff at 0.5) + flash ---- should probably clamp
    // a: use red channel of texture * distance-based fade-in/out
    fragColour = vec4(clamp(colour * min(1.0, texCoord.t + 0.5) + max(0.0, flash), 0.0, 1.0), texture(cloudTexture, texCoord).r * alpha);
}
