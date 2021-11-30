#version 460
in vec2 TexCoord;

out vec4 color;

uniform sampler2D tex1;

void main()
{
    color = texture(tex1, TexCoord);
    color = vec4(1.0-color.r, 1.0-color.g, 1.0-color.b, color.a);
}
