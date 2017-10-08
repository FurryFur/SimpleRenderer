#version 450 core

in VertexData {
    vec4 normal;
    vec2 texCoord;
    vec3 viewPos;
} i;

out vec4 outColor;

uniform sampler2D sampler;

const vec3 lightDir = vec3(0, 0, 1);
const vec3 Li = vec3(1, 1, 1);
const vec3 ambient = vec3(0.1, 0.1, 0.1);
const vec3 kTransmittance = vec3(0.5, 0.5, 0.5);
const float specPow = 5.0f;

void main(void)
{
    vec3 normal;
    if (gl_FrontFacing)
        normal = normalize(i.normal.xyz);
    else
        normal = -normalize(i.normal.xyz);

    float ldotn = clamp(dot(normalize(lightDir), normal), 0, 1);

    vec3 reflDir = reflect(-lightDir, normal);
    vec3 viewDir = normalize(-i.viewPos);
    float vdotr =  clamp(dot(viewDir, reflDir), 0, 1);

    vec3 Lr = Li * pow(vdotr, specPow) * ldotn;

    outColor = texture(sampler, i.texCoord);
    outColor = vec4(ambient + Lr, 1) * outColor;

    // if (abs(ldotn) < 0.5)
    //     discard;
}
