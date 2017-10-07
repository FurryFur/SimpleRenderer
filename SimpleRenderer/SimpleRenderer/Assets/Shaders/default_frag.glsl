#version 450 core

in VertexData {
    vec4 normal;
    vec2 texCoord;
} i;

out vec4 outColor;

uniform sampler2D sampler;

const vec3 lightDir = vec3(0, 0, 1);
const vec3 lightCol = vec3(1, 1, 1);
const vec3 ambient = vec3(0.1, 0.1, 0.1);

void main(void)
{
    vec3 normal = normalize(i.normal.xyz);
    float lightIntensity = clamp(dot(normalize(lightDir), normal), 0, 1);
    outColor = texture(sampler, i.texCoord);

    outColor = (vec4(ambient, 1) + lightIntensity * vec4(lightCol, 1)) * outColor;
}
