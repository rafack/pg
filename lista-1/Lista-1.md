### LISTA 1 — Exercícios

**1. O que é a GLSL? Quais os dois tipos de shaders são obrigatórios no pipeline programável da versão atual que trabalhamos em aula e o que eles processam?**

A GLSL é a linguagem de programação dos shaders para o processamento gráfico. Os shaders obrigatórios são:

- *vertex shader*: responsável pelo processamento dos vértices e onde eles estarão posicionados na tela (coordenadas). Roda para cada vértice;
- *fragment shader*: roda para cada "pixel", determinando sua cor.

**2. O que são primitivas gráficas? Como fazemos o armazenamento dos vértices na OpenGL?**

São as formas geométricas enviadas para serem desenhadas, como *GL_POINTS*, *GL_TRIANGLES* e *GL_LINE_STRIP*. Os vértices são armazenados em buffers.

**3. Explique o que é VBO, VAO e EBO, e como se relacionam (se achar mais fácil, pode fazer um gráfico representando a relação entre eles).**

VBO é um buffer utilizado para armazenar os vértices e enviá-los para a GPU.

VAO é um buffer para armazenamento dos atributos dos vértices, as chamadas para habilitar estes atributos e os VBOs (ou EBOs) associados aos mesmos.

EBO tem o mesmo objetivo que o VBO, porém ele armazena os índices que representam cada vértice (do VBO).

**4. Considerando o seguinte triângulo abaixo, formado pelos vértices P1, P2 e P3, respectivamente com as cores vermelho, verde e azul.**

**a. Descreva uma possível configuração dos buffers (VBO, VAO e EBO) para representá-lo.**

```cpp
int setupGeometry()
{
    GLfloat vertices[] = {
    // posições
    -0.75, -0.625, 0.0,
    0.75, -0.375, 0.0,
    0.0, 0.75, 0.0,
    // cores
    0.0, 1.0, 0.0,
    0.0, 0.0, 1.0,
    1.0, 0.0, 0.0,
    };

    GLuint VBO, VAO;

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(9 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBindVertexArray(0);

    return VAO;
}
```

**b. Como estes atributos seriam identificados no vertex shader?**

```glsl
#version 450
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec4 vertexColor;

void main()
{
    gl_Position = vec4(position.x, position.y, position.z, 1.0);
    vertexColor = vec4(color, 1.0);
}
```

**5. Analise o código fonte do projeto Hello Triangle. Localize e relacione os conceitos de shaders, VBOs e VAO apresentados até então. Não precisa entregar nada neste exercício.**

**6. Faça o desenho de 2 triângulos na tela. Desenhe eles:**

**a. Apenas com o polígono preenchido**

**b. Apenas com contorno**

**c. Apenas como pontos**

**d. Com as 3 formas de desenho juntas**


**7. Faça o desenho de um círculo na tela.**


**8. Faça o passo-a-passo para criar o triângulo com cores diferentes em cada vértice (prática do exercício 5).**


**9. Faça um desenho em um papel quadriculado (pode ser no computador mesmo) e reproduza-o utilizando primitivas em OpenGL. Neste exercício você poderá criar mais de um VAO e fazer mais de uma chamada de desenho para poder utilizar primitivas diferentes, se necessário.**


**10. Implemente (pode pegar do tutorial) uma classe para tratar os shaders a partir de arquivos.**
