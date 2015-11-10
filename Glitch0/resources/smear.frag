#version 430

layout(location = 0) in vec2 texCoord;

layout(location = 0) uniform sampler2D colour;
layout(location = 1) uniform sampler2D depth;
layout(location = 2) uniform sampler2D noise0;
layout(location = 3) uniform sampler2D noise1;
layout(location = 4) uniform sampler2D lastFrame;

layout(location = 5) uniform mat4 view;

layout(location = 20) uniform float rand0;
layout(location = 21) uniform float rand1;
layout(location = 22) uniform float tickTime;

layout(location = 0) out vec4 fragColour;

void main()
{
    //vec2 offset = texture(noise1, texCoord + vec2(rand0, rand1)).xy - vec2(0.35, 0.35);
    vec2 offset = texture(noise1, texCoord).xy - vec2(0.35, 0.35);
    offset = normalize(offset) * tickTime * 0.00001;
    //offset = offset / 128.0;
    vec2 t = texCoord + offset;
    fragColour = texture(lastFrame, t) * (0.75 + 0.25 * fract(rand1));
    //fragColour.g = 0.0;
    fragColour.a = 0.999 * tickTime;
}
