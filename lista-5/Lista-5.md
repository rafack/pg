### LISTA 5 — Exercícios

**1. Modifique o fragment shader para criar os seguintes "filtros" (último exemplo da aula passada)**

**a. Renderizar apenas o canal vermelho, verde ou azul separadamente**
[fragment_shader-exercicio1a](./HelloOrtho/Exercicio7/shaders/fragment_shader-exercicio1a.fs)

Implementei no `Origem.cpp` uma lógica para trocar o canal que será renderizado através das teclas "R", "G" e "B".

**b. Grayscale com média ponderada**
[fragment_shader-exercicio1b](./HelloOrtho/Exercicio7/shaders/fragment_shader-exercicio1b.fs)

**c. Colorização, mandando como uma uniform a cor a acrescentar**
[fragment_shader-exercicio1c](./HelloOrtho/Exercicio7/shaders/fragment_shader-exercicio1c.fs)

**d. Inversão**
[fragment_shader-exercicio1d](./HelloOrtho/Exercicio7/shaders/fragment_shader-exercicio1d.fs)

**e. Binarização**
[fragment_shader-exercicio1e](./HelloOrtho/Exercicio7/shaders/fragment_shader-exercicio1e.fs)

**f. Crie um filtro seu! (pode combinar filtros já feitos)**
[fragment_shader-exercicio1f](./HelloOrtho/Exercicio7/shaders/fragment_shader-exercicio1f.fs)

**g. (DESAFIO) Efeito de "vignete" (vide imagem .png de exemplo no Moodle)**
[fragment_shader-exercicio1g](./HelloOrtho/Exercicio7/shaders/fragment_shader-exercicio1g.fs)


**2. Até agora estamos aplicando isso nas texturas dos nossos objetos e usando apenas um shader. O desafio é: mostre quatro ou mais dos filtros implementados na mesma cena (mostre lado a lado os diferentes filtros).**

[exercicio2](./HelloOrtho/Exercicio7/)