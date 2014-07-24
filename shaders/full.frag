#version 410

layout(location = 0) in vec3 normal;
layout(location = 1) in vec3 viewspacePosition;
layout(location = 2) in vec3 viewspaceNormal;

layout(location = 0) out vec4 fragColour;

uniform mat4 v;

uniform vec3 colour;
uniform float specular;
uniform float shininess;
uniform float reflectiveness;

uniform samplerCube environment;

void main()
{
    vec3 n = normalize(normal);
    vec3 vp = normalize(viewspacePosition);
    vec3 vn = normalize(viewspaceNormal);
    
    vec3 lightDir = normalize(vec3(-4.0, 0.0, -4.0));
    
    float diffuse = max(0.0, dot(n, -lightDir));
    
    vec3 rofl = reflect(vp, vn);
    rofl = (inverse(v) * vec4(rofl, 0.0)).xyz;
    vec3 reflected = texture(environment, rofl).rgb;
    
    vec3 lightReflect = reflect(-lightDir, n);
    float s = max(0.0, dot(lightReflect, vp));
    s = 0.5 * pow(s, shininess);
    
    fragColour = vec4(((1.0 - reflectiveness) * diffuse + reflectiveness * reflected) * colour + vec3(s) * specular, 1.0);
}
