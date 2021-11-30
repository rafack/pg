#version 460
in vec2 TexCoord;

out vec4 color;

uniform sampler2D tex1;

void main()
{
    color = texture(tex1, TexCoord);
    float media = color.r * 0.2125 + color.g * 0.7154 + color.b * 0.0721;
    color = vec4(media,media,media,color.a);
}
