#version 430

layout(location = 0) in vec3 vertex;

layout(location = 0) out vec3 texCoord;

uniform mat4 m;
uniform mat4 v;
uniform mat4 p;

void main()
{
    mat4 vv = v;
    vv[3] = vec4(0.0, 0.0, 0.0, 1.0);
    texCoord = vertex;
    gl_Position = p * vv * vec4(vertex, 1.0);
}
