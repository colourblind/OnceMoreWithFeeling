#version 400

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;

out vec3 lightIntensity;

uniform mat4 m;
uniform mat4 v;
uniform mat4 p;

void main()
{
    vec3 lightDir = normalize(vec3(0.0, 1.0, 0.5));
    mat3 normalMatrix = transpose(inverse(mat3(m)));
    lightIntensity = vec3(dot(normalMatrix * normal, lightDir));
    gl_Position = p * v * m * vec4(vertex, 1.0);
}
