#include <iostream>
#include <string>
#include <assert.h>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
// diamond geometry
int d_setupGeometry();

// circle geometry
int c_setupGeometry();
const int qtdVertices = 180;

const GLuint WIDTH = 900, HEIGHT = 600;

int nvertices = 100 + 1 + 1; //+ centro + cópia do 1º


int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ola Triangulo!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);


	// GLAD: carrega todos os ponteiros de funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	// Obtendo as informações de versão
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;


	int width, height;


	// Compilando e buildando o programa de shader
	Shader* shader = new Shader("./shaders/ortho.vs", "./shaders/ortho.fs");

	GLuint dVAO = d_setupGeometry();
	GLuint cVAO = c_setupGeometry();

	shader->Use();

	glm::mat4 projection = glm::mat4(1);
	projection = glm::ortho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);


	// "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();

		// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		// Limpa o buffer de cor
		glClearColor(0.0, 0.611765, 0.231373, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(10);

		shader->setVec4("inputColor", 1.0, 0.874510, 0.0);
		glBindVertexArray(dVAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		shader->setVec4("inputColor", 0.0, 0.152941, 0.462745);
		glBindVertexArray(cVAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, qtdVertices * 3 + 1);
		glBindVertexArray(0);


		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &dVAO);

	// Finaliza a execução da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Função de callback de teclado - só pode ter uma instância (deve ser estática se
// estiver dentro de uma classe) - É chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}


int d_setupGeometry()
{
	GLfloat vertices[] = {
		0.0, 0.7666665, 0.0,//topo
		-0.7666665, 0.0, 0.0,//esquerda
		0.0, -0.7666665, 0.0,//baixo
		0.7666665, 0.0, 0.0,//direita
	};


	unsigned int indices[] = {
	0,1,2,
	2,3,0
	};


	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);


	return VAO;
}

int c_setupGeometry()
{
	float angulo = 0.0;
	float raio = 0.4;
	float circunferencia = 2 * M_PI;
	float deltaAngulo = circunferencia / qtdVertices;

	GLfloat* vertices = new GLfloat[(qtdVertices + 3) * 3];

	int i = 0;

	while (angulo <= circunferencia) {
		int deslocamentoNoArray = i * 3;
		angulo = (float)i * deltaAngulo;

		vertices[deslocamentoNoArray] = raio * cos(angulo);
		vertices[deslocamentoNoArray + 1] = raio * sin(angulo);
		vertices[deslocamentoNoArray + 2] = 0.0;

		i++;
	}

	vertices[qtdVertices * 3] = 0.0;
	vertices[qtdVertices * 3 + 1] = 0.0;
	vertices[qtdVertices * 3 + 2] = 0.0;

	unsigned int VBO, VAO;

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, ((qtdVertices * 3) + 1) * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBindVertexArray(0);

	return VAO;
}