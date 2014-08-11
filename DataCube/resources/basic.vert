#version 410

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 colour;

layout(location = 0) out vec3 colourOut;

uniform mat4 m;
uniform mat4 v;
uniform mat4 p;

void main()
{
    colourOut = colour;
    gl_Position = p * v * m * vec4(vertex, 1.0);
}
