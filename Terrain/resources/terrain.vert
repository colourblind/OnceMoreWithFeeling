#version 430

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 types;
layout(location = 4) in vec2 texCoord2;

uniform mat4 m;
uniform mat4 v;
uniform mat4 p;

layout(location = 0) out vec3 normalOut;
layout(location = 1) out vec2 texCoordOut;
layout(location = 2) out vec3 typesOut;
layout(location = 3) out vec2 texCoord2Out;

void main()
{
    mat3 normalMatrix = transpose(inverse(mat3(m)));
    normalOut = normalMatrix * normal;
    
    texCoordOut = texCoord;
    typesOut = types;
    texCoord2Out = texCoord2;
    
    gl_Position = p * v * m * vec4(vertex, 1.0);
}
