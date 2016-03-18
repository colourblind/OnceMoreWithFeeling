#version 430

layout(location = 0) in vec2 texCoord;

layout(location = 0) uniform sampler2D colour;
layout(location = 1) uniform sampler2D depth;
layout(location = 2) uniform sampler2D noise0;
layout(location = 3) uniform sampler2D noise1;
layout(location = 6) uniform sampler2D noise2;

layout(location = 5) uniform mat4 view;

layout(location = 0) out vec4 fragColour;
layout(location = 1) out vec4 saveColour;

//const int NUM_SAMPLES = 6;
const float DECAY = 0.05;
const float WEIGHT = 2.0;
const float SCALE = 0.5;

vec4 radial()
{
    int NUM_SAMPLES = int(pow(texture(noise2, (mat2(view) * texCoord).ts).r, 2.0) * 40.0);

    vec2 src = mat2(view) * texCoord; //clamp(vec2(0.25, 0.5);
    vec2 dst = texCoord;
    vec2 step = (dst - src) / float(NUM_SAMPLES) * SCALE;
 
    vec4 result = vec4(0.0, 0.0, 0.0, 1.0);
    float falloff = 1.0;
    for (int i = 0; i < NUM_SAMPLES; i ++)
    {
        dst -= step;
        vec4 t = texture(colour, dst) * falloff * WEIGHT;
        
        vec2 t0 = mat2(view) * dst;
        float foo = texture(noise2, t0).r;

        result += vec4(t.rgb, 0.0);// * clamp(foo, 0.0, 1.0);
        
        falloff *= 1.0 - (DECAY / NUM_SAMPLES) ;
    }
    
    vec4 final = clamp(result / NUM_SAMPLES, vec4(0.0), vec4(1.0));
    final.a = 0.75;
    
    //vec2 t0 = mat2(view) * texCoord;
    //fragColour.a = texture(noise1, t0).r * 0.5;
    //fragColour = texture(glow, texCoord);
    
    return final;
}

void main()
{
    fragColour = texture(colour, texCoord);
    saveColour = fragColour;
    vec4 rad = radial();
    fragColour.rgb = clamp(fragColour.rgb + (rad.rgb * rad.a), vec3(0.0), vec3(1.0));
}
