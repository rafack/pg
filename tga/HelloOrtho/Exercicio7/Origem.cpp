#include <assert.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <math.h>
#include <string>
#include "Object.h"
#include "Shader.h"
#include "stb_image.h"

#define METADE_ALTURA_PERSONAGEM 0.105257
#define METADE_LARGURA_PERSONAGEM 0.042000
#define METADE_LARGURA_BOLINHA 0.040909
#define METADE_ALTURA_BOLINHA 0.064286

#define LIMITE_INFERIOR_JANELA -1.0

using namespace std;

const GLuint WIDTH = 1100, HEIGHT = 700;

GLFWwindow* window;

Shader* shader;

Object background;
Object personagem;
Object bolinha;

bool gameover = false;
int pontuacao = 0;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

void carregarGraficos();
void inicializarObjetos();

void verificarColisao();
void tratarColisao();

void finalizarJogo();

int main()
{
	carregarGraficos();

	shader = new Shader();
	shader->use();

	inicializarObjetos();

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		glClearColor(1, 1, 1, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::mat4(1);
		projection = glm::ortho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
		shader->setMat4("projection", glm::value_ptr(projection));
		
		background.update();
		background.draw();
		
		personagem.update();
		personagem.draw();	
		
		if (!gameover)
		{
			if (bolinha.getPosY() > LIMITE_INFERIOR_JANELA) {
				bolinha.setPosition(glm::vec3(bolinha.getPosX(), bolinha.getPosY() - 0.0005, 0.0));
			}
			else
			{
				finalizarJogo();
			}
			
			bolinha.update();
			bolinha.draw();
			
			verificarColisao();
		}

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_RIGHT)
	{
		personagem.moverParaDireita();
	}

	if (key == GLFW_KEY_LEFT)
	{
		personagem.moverParaEsquerda();
	}

	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
	{
		gameover = false;
		inicializarObjetos();
		pontuacao = 0;
	}

	personagem.update();
	personagem.draw();
}

void carregarGraficos()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Ola, TrabGA!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;
}

void inicializarObjetos()
{
	background.initialize();
	background.setShader(shader);
	background.setTexture("./textures/room.jpg");
	background.setDimensions(glm::vec3(2.0, 2.0, 0.0));

	personagem.initialize();
	personagem.setShader(shader);
	personagem.setTexture("./textures/courage2.png");
	personagem.setDimensions(glm::vec3(0.35, 0.48, 1.0));
	personagem.setPosition(glm::vec3(-0.5, -0.5, 0.0));

	bolinha.initialize();
	bolinha.setShader(shader);
	bolinha.setTexture("./textures/bolinha.png");
	bolinha.setDimensions(glm::vec3(0.18, 0.18, 0.3));
	bolinha.setPosition(glm::vec3(0.0, 1.0, 0.0));
	bolinha.posicaoAleatoriaEmX();
}

void verificarColisao()
{
	double limiteSuperiorPersonagem = personagem.getPosY() - METADE_ALTURA_PERSONAGEM;
	double limiteInferiorPersonagem = personagem.getPosY() + METADE_ALTURA_PERSONAGEM;

	double limiteSuperiorBolinha = bolinha.getPosY() - METADE_ALTURA_BOLINHA;
	double limiteInferiorBolinha = bolinha.getPosY() + METADE_ALTURA_BOLINHA;

	if (limiteSuperiorBolinha <= limiteInferiorPersonagem && limiteInferiorBolinha >= limiteSuperiorPersonagem)
	{
		double limiteEsquerdoPersonagem = personagem.getPosX() - METADE_LARGURA_PERSONAGEM;
		double limiteDireitoPersonagem = personagem.getPosX() + METADE_LARGURA_PERSONAGEM;

		double limiteEsquerdoBolinha = bolinha.getPosX() - METADE_LARGURA_BOLINHA;
		double limiteDireitoBolinha = bolinha.getPosX() + METADE_LARGURA_BOLINHA;

		if ((limiteEsquerdoBolinha > limiteEsquerdoPersonagem && limiteEsquerdoBolinha < limiteDireitoPersonagem)
			|| (limiteDireitoBolinha < limiteDireitoPersonagem && limiteDireitoBolinha > limiteEsquerdoPersonagem))
		{
			tratarColisao();
		}
	}
}

void tratarColisao()
{
	pontuacao++;
	bolinha.posicaoAleatoriaEmX();
}

void finalizarJogo() {
	gameover = true;

	printf("Sua pontuacao foi: %d\n", pontuacao);

	bolinha.setPosition(glm::vec3(-2.0, -2.0, 0.0));
	bolinha.update();
	bolinha.draw();

	background.setTexture("./textures/the-end.png");
	background.update();
	background.draw();

	personagem.setPosition(glm::vec3(-2.0, -2.0, 0.0));
	personagem.update();
	personagem.draw();
}