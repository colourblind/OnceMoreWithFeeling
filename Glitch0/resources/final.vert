#version 430

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec2 texCoord;

uniform mat4 m;
uniform mat4 v;
uniform mat4 p;

layout(location = 0) out vec2 texCoordOut;

void main()
{
    texCoordOut = texCoord;
    gl_Position = p * v * m * vec4(vertex, 1.0);
}
