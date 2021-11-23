#version 460
in vec2 TexCoord;

out vec4 color;

uniform sampler2D tex1;

void main()
{
    color = texture(tex1, TexCoord);
    float media = color.r * 0.2125 + color.g * 0.7154 + color.b * 0.0721;
    color = vec4(media,media,media,color.a);

    float limiar = 0.275;

    if(color.r >= limiar)
    {
        color.r = 1.0;
    }
    else
    {
        color.r = 0.0;
    }

    if(color.g >= limiar)
    {
        color.g = 1.0;
    }
    else
    {
        color.g = 0.0;
    }

        if(color.b >= limiar)
    {
        color.b = 1.0;
    }
    else
    {
        color.b = 0.0;
    }

}
