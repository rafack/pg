#version 460
in vec2 TexCoord;

out vec4 color;

uniform sampler2D tex1;
uniform vec4 corColorizadora;

void main()
{
    color = texture(tex1, TexCoord);
    color = color + corColorizadora;
}
