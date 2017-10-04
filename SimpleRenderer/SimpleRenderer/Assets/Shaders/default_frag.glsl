#version 450 core

in VertexData {
    vec2 texCoord;
} i;

out vec4 outColor;

uniform sampler2D sampler;

void main(void)
{
    outColor = vec4(i.texCoord, 0, 1); //texture(sampler, i.texCoord);
}
