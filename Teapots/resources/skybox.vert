#version 410

layout(location = 0) in vec3 vertex;

layout(location = 0) out vec3 viewspacePosition;

uniform mat4 m;
uniform mat4 v;
uniform mat4 p;

void main()
{
    viewspacePosition = (v * m * vec4(vertex, 1.0)).xyz;
    gl_Position = p * v * m * vec4(vertex, 1.0);
}
