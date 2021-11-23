#version 460
in vec2 TexCoord;

uniform sampler2D tex1;
uniform vec2 dimensoesTela;

const float raio = 0.65, intensidade = 0.9;
vec2 resolucao = vec2(dimensoesTela);

void main()
{
    vec4 color = texture(tex1, TexCoord);

    vec2 relativePosition = gl_FragCoord.xy / resolucao - .5;
	float tamanho = length(relativePosition);

    float efeitoVignette = smoothstep(raio, 0.0, tamanho);
	color.rgb = mix(color.rgb, color.rgb * efeitoVignette, intensidade);

	gl_FragColor = color;
}
