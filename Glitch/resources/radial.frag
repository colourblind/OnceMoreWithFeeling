#version 430

layout(location = 0) in vec2 texCoord;

layout(location = 3) uniform sampler2D noise1;
layout(location = 4) uniform sampler2D glow;

layout(location = 5) uniform mat4 view;

out vec4 fragColour;

//const int NUM_SAMPLES = 6;
const float DECAY = 0.05;
const float WEIGHT = 2.0;
const float SCALE = 0.5;

void main()
{
    int NUM_SAMPLES = int(pow(texture(noise1, (mat2(view) * texCoord).ts).r, 2.0) * 40.0);
 
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

        result += vec4(t.rgb, 0.0) * clamp(foo, 0.0, 1.0);
        
        falloff *= 1.0 - (DECAY / NUM_SAMPLES) ;
    }
    
    fragColour = clamp(result / NUM_SAMPLES, vec4(0.0), vec4(1.0));
    fragColour.a = 0.75;
    
    //vec2 t0 = mat2(view) * texCoord;
    //fragColour.a = texture(noise1, t0).r * 0.5;
    //fragColour = texture(glow, texCoord);
}
