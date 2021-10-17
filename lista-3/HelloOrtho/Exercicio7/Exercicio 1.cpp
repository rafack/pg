#include <iostream>
#include <string>
#include <assert.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

using namespace std;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
int setupGeometry();

const GLuint WIDTH = 900, HEIGHT = 600;

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ola, Lista 3!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da funcao de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);


	// GLAD: carrega todos os ponteiros de funcoes da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	// Obtendo as informacoes de versao
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;


	int width, height;


	// Compilando e buildando o programa de shader
	Shader* shader = new Shader();
	shader->use();

	glm::mat4 projection = glm::mat4(1);
	glm::mat4 trans = glm::mat4(1);
	glm::mat4 rotate = glm::mat4(1);
	glm::mat4 scale = glm::mat4(1);

	projection = glm::ortho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	trans = glm::ortho(1.3, -1.3, -1.3, 1.3, -1.0, 1.0);
	rotate = glm::rotate(projection, glm::radians(180.0f), glm::vec3(0.0, 0.0, 1.0));
	scale = glm::scale(projection, glm::vec3(0.4, 0.3, 1.0));

	GLuint VAO = setupGeometry();

	// "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funcoes de callback correspondentes
		glfwPollEvents();

		// Definindo as dimensoes da viewport com as mesmas dimensoes da janela da aplicacao
		glfwGetFramebufferSize(window, &width, &height);

		// Limpa o buffer de cor
		glClearColor(1.0, 0.0, .3, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);

		shader->setVec4("inputColor", 0.2, 0.6, 0.9);
		glBindVertexArray(VAO);


		shader->setMat4("projection", glm::value_ptr(trans));
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		
		shader->setVec4("inputColor", 0.2, 0.0, 0.2);
		shader->setMat4("projection", glm::value_ptr(rotate));
		glDrawArrays(GL_TRIANGLES, 0, 3);


		shader->setVec4("inputColor", 0.9, 0.8, 0.2);
		shader->setMat4("projection", glm::value_ptr(scale));
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO);

	// Finaliza a execucao da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Funcao de callback de teclado - so pode ter uma instancia (deve ser estetica se
// estiver dentro de uma classe) - e chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}


int setupGeometry()
{
	GLfloat vertices[] = {
		0.0, 0.88, 0.0,//topo
		-0.45, 0.0, 0.0,//esquerda
		0.45, 0.0, 0.0,//direita
	};


	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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
