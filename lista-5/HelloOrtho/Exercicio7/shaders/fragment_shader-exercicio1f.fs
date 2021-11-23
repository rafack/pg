#version 460
in vec2 TexCoord;

out vec4 color;

uniform sampler2D tex1;
uniform vec4 corColorizadora;

void main()
{
    color = texture(tex1, TexCoord);
    color = vec4(color.r * 0.2125, color.g * 0.7154, color.b * 0.0721, color.a) + corColorizadora;
}
