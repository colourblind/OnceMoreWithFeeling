#version 430

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

layout(location = 0) out float diffuse;
layout(location = 1) out vec2 texCoordOut;

uniform mat4 m;
uniform mat4 v;
uniform mat4 p;

layout(location = 4) uniform mat4 view;

void main()
{
    float nscale = fract((mat3(view) * vertex).x * 3.142) > 0.95 ? -1.0 : 1.0;
    vec3 v0 = vertex;
    vec3 n0 = (inverse(view) * vec4(normal * nscale, 0.0)).xyz;

    vec3 worldspacePosition = (m * vec4(v0, 1.0)).xyz;

    vec3 lightPosition = vec3(0.2, 0.5, 8);
    vec3 lightDir = normalize(worldspacePosition - lightPosition);
    
    mat3 normalMatrix = transpose(inverse(mat3(m)));
    diffuse = max(0.0, dot(normalize(normalMatrix * n0), -lightDir));
    
    texCoordOut = texCoord;
    gl_Position = p * v * m * vec4(v0, 1.0);
}
