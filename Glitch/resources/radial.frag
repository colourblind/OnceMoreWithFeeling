#version 430

layout(location = 0) in vec2 texCoord;

layout(location = 3) uniform sampler2D noise1;
layout(location = 4) uniform sampler2D glow;

layout(location = 5) uniform mat4 view;

out vec4 fragColour;

const int NUM_SAMPLES = 20;
const float DECAY = 0.97;
const float WEIGHT = 0.1;
const float SCALE = 0.5;

void main()
{
    vec2 src = vec2(0.25, 0.5);
    vec2 dst = texCoord;
    vec2 step = (dst - src) / float(NUM_SAMPLES) * SCALE;
 
    vec4 result = vec4(0.0, 0.0, 0.0, 1.0);
    float falloff = 1.0;
    for (int i = 0; i < NUM_SAMPLES; i ++)
    {
        dst -= step;
        vec4 t = texture(glow, dst) * falloff * WEIGHT;
        
        vec2 t0 = mat2(view) * dst;
        float foo = texture(noise1, t0).r;

        result += vec4(t.rgb, 0.0) * clamp(foo - 0.25, 0.0, 1.0);
        
        falloff *= DECAY;
    }
    
    fragColour = clamp(result, vec4(0.0), vec4(1.0));
    fragColour.a = 0.75;
    
    //vec2 t0 = mat2(view) * texCoord;
    //fragColour.a = texture(noise1, t0).r * 0.5;
    //fragColour = texture(glow, texCoord);
}
