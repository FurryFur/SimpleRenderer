#version 450 core

in VertexData {
    vec4 normal;
    vec2 texCoord;
} i;

out vec4 outColor;

uniform sampler2D sampler;

const vec3 lightDir = vec3(0, 0, 1);
const vec3 lightCol = vec3(1, 1, 1);

void main(void)
{
    vec3 normal = normalize(i.normal.xyz);
    float lightIntensity = dot(normalize(lightDir), normal);

    outColor = vec4(lightIntensity * lightCol * vec3(0.5, 0.5, 0.5), 1); //texture(sampler, i.texCoord);
}
