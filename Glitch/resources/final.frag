#version 430

layout(location = 0) in vec2 texCoord;

layout(location = 0) uniform sampler2D colour;
layout(location = 1) uniform sampler2D depth;
layout(location = 2) uniform sampler2D noise0;
layout(location = 3) uniform sampler2D noise1;
layout(location = 4) uniform sampler2D glow;

layout(location = 5) uniform mat4 view;

out vec4 fragColour;

void main()
{
    vec2 t0 = mat2(view) * texCoord;
    vec2 t1 = mat2(transpose(view)) * texCoord;
    vec2 t2 = mat2(inverse(view)) * texCoord;
    
    float c = texture(colour, texCoord).r;
    float d = texture(depth, t0).r;
    float n0 = texture(noise0, t1).r;
    float n1 = texture(noise1, t2).r;
    float g = texture(glow, texCoord).r;
    
    float f = clamp(c * 0.75 + (g * n1) * 0.25, 0.0, 1.0);

    fragColour = vec4(f, f, f, n0 * d);
}
