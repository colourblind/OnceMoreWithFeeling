#version 410

layout(location = 0) in vec3 vertex;

uniform mat4 m;
uniform mat4 v;
uniform mat4 p;

uniform vec3 colour;

void main()
{
    vec3 flap = vertex * vec3(1.0, cos(colour.b * 2 * 3.14159265), 1.0);
    gl_Position = p * v * m * vec4(flap, 1.0);
}
