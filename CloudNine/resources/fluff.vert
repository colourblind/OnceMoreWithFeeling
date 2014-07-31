#version 430

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec2 texCoord;

layout(location = 0) out vec3 colourOut;
layout(location = 1) out vec3 worldPosition;
layout(location = 2) out vec2 texCoordOut;

uniform mat4 m;
uniform mat4 v;
uniform mat4 p;

layout(location = 0) uniform vec3 lightDir;
layout(location = 3) uniform vec3 normal;

uniform vec3 colour;

void main()
{
    gl_Position = p * v * m * vec4(vertex, 1.0);
    worldPosition = (m * vec4(vertex, 1.0)).xyz;

    float diffuse = max(0.5, dot(normal, normalize(-lightDir)));
    colourOut = colour.rrr * diffuse;
    
    texCoordOut = texCoord;
}
