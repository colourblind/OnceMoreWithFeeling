#version 400

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;

out vec3 lightIntensity;

uniform mat4 m;
uniform mat4 v;
uniform mat4 p;

void main()
{
    lightIntensity = vec3(1.0, 1.0, 1.0);
    gl_Position = p * m * v * vec4(vertex, 1.0);
}
