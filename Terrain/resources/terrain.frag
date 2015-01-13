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
    float diffuse = max(0.0, dot(n, -lightDir));
    
    float light = 0.4 + 0.6 * diffuse;

    vec3 grassColour = texture(grass, texCoord).rgb;
    vec3 snowColour = texture(snow, texCoord).rgb;
    vec3 rockColour = texture(rock, texCoord).rgb;
    float mask = texture(mask, texCoord).g;
    vec3 grassColour2 = texture(grass, texCoord2).rgb;
    vec3 snowColour2 = texture(snow, texCoord2).rgb;
    vec3 rockColour2 = texture(rock, texCoord2).rgb;

    vec3 t = types / (types.x + types.y + types.z);
    fragColour = vec4(
        (
            ((grassColour * t.x + rockColour * t.y + snowColour * t.z) * (1.0 - mask)) 
            + ((grassColour2 * t.x + rockColour2 * t.y + snowColour2 * t.z) *  mask)
        ) * colour * light, 1.0);
}
