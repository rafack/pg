#version 460
in vec2 TexCoord;

out vec4 color;

uniform sampler2D tex1;
uniform int cor;

void main()
{
    color = texture(tex1, TexCoord);
    color = vec4(color.r, color.g, color.b, color.a);

    if(cor == 0)
    {
        color = vec4(color.r, color.r, color.r, color.a);
    }

    else if(cor == 1)
    {
        color = vec4(color.g, color.g, color.g, color.a);
    }

    else if(cor == 2)
    {
        color = vec4(color.b, color.b, color.b, color.a);
    }
}
