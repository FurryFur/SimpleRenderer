#version 450 core

in VertexData {
    vec2 texCoord;
} i;

out vec4 outColor;

uniform sampler2D texture;

void main(void)
{
    outColor = texture(texture, i.texCoord);
}
