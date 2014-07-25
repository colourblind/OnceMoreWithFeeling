#version 410

layout(location = 0) in vec2 vertex;
layout(location = 1) in vec2 texCoord;

layout(location = 0) out vec2 texCoordOut;

uniform mat4 m;
uniform mat4 v;
uniform mat4 p;

void main()
{
    texCoordOut = texCoord;
    gl_Position = p * v * m * vec4(vertex, 0.0, 1.0);
}
