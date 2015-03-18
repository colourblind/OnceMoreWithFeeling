#version 430

layout(location = 0) in vec2 texCoord;

layout(location = 0) uniform sampler2D colour;
layout(location = 1) uniform sampler2D depth;

out vec4 fragColour;

void main()
{
    // Change this to 0.1 for glitchfun, 0.0025 for something better
    // TODO: make this depend on FBO dimensions
    float DEPTH_OFFSETS = 0.0025;
    float EDGE_DETECTION_SCALE = 200.0;

    vec4 c = texture(colour, texCoord);
    
    float d0 = texture(depth, texCoord).r;
    float d1 = texture(depth, texCoord + vec2(0.0, -DEPTH_OFFSETS)).r;
    float d2 = texture(depth, texCoord + vec2(0.0, DEPTH_OFFSETS)).r;
    float d3 = texture(depth, texCoord + vec2(-DEPTH_OFFSETS, 0.0)).r;
    float d4 = texture(depth, texCoord + vec2(DEPTH_OFFSETS, 0.0)).r;
    
    float diff = abs(d0 - d1) + abs(d0 - d2) + abs(d0 - d3) + abs(d0 - d4);
    diff = clamp(0.0, 1.0, diff * d0 * EDGE_DETECTION_SCALE);
    
    vec4 d = vec4(diff, diff, 0.0, 1.0);

    fragColour = clamp(vec4(0.0, 0.0, 0.0, 1.0), vec4(1.0, 1.0, 1.0, 1.0), c + d);
}
