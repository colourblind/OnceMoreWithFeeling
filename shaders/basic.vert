#version 410

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;

layout(location = 0) out vec3 normalOut;
layout(location = 1) out vec3 worldspacePosition;

uniform mat4 m;
uniform mat4 v;
uniform mat4 p;

uniform vec3 eyePosition;

void main()
{
    mat3 normalMatrix = transpose(inverse(mat3(m)));
    normalOut = normalMatrix * normal;
    gl_Position = p * v * m * vec4(vertex, 1.0);
    worldspacePosition = (m * vec4(vertex, 1.0)).xyz;
}
