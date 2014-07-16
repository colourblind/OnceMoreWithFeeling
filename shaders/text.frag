#version 410

layout(location = 0) in vec2 texCoord;

layout(location = 0) out vec4 fragColour;

uniform vec3 colour;
uniform sampler2D texture;

void main()
{
    //vec3 finalColour = vec3(texture(texture, texCoord)) * colour;
    //fragColour = vec4(finalColour, 1.0);
    fragColour = vec4(texture(texture, texCoord).rrr, 1.0);
}
