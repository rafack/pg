#include <assert.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <string>
#include <vector>
#include "Object.h"
#include "Shader.h"
#include "stb_image.h"

using namespace std;

const GLuint WIDTH = 1280, HEIGHT = 720;
GLFWwindow* window;

Shader* shader;

bool arquivoInformado;

Object telaInicial;
Object barraSuperior;
Object imagem;
vector <Object> stickers;
const int QTD_STICKERS = 11;
float espacoEntreStickers = 0.18;
bool verificarCliqueTela;
double xTela, yTela;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
static void cursor_position_callback(GLFWwindow* window, double xTela, double yTela);

void carregarGraficos();
void iniciarObjetos();
void solicitarArquivoImagem();
void mostrarTelaInicial();
void definirTexturaStickers();

int main()
{
	carregarGraficos();

	shader = new Shader();
	shader->use();

	iniciarObjetos();

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
		
		if (!arquivoInformado)
		{
			mostrarTelaInicial();
			glfwSwapBuffers(window);

			solicitarArquivoImagem();
		}

		else
		{
			imagem.update();
			imagem.draw();

			barraSuperior.update();
			barraSuperior.draw();


			for (int i = 0; i < stickers.size(); i++)
			{
				stickers[i].update();
				stickers[i].draw();
			}
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
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		if (arquivoInformado)
		{
			verificarCliqueTela = true;
		}
	}
}

static void cursor_position_callback(GLFWwindow* window, double xTela, double yTela)
{
	if (verificarCliqueTela) 
	{
		glfwGetCursorPos(window, &xTela, &yTela);
		printf("x: %d y: %d", xTela, yTela);

		verificarCliqueTela = false;
	}
}

void carregarGraficos()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Ola, TrabGB!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;
}

void iniciarObjetos()
{
	telaInicial.initialize();
	telaInicial.setShader(shader);
	telaInicial.setTexture("./textures/tela-inicial.png");
	telaInicial.setDimensions(glm::vec3(2.0,2.0,1.0));
	
	barraSuperior.initialize();
	barraSuperior.setShader(shader);
	barraSuperior.setTexture("./textures/barra-superior.png");
	barraSuperior.setPosition(glm::vec3(-0.6, 0.8, 1.0));
	barraSuperior.setDimensions(glm::vec3(3.2,0.4,1.0));
	
	imagem.initialize();
	imagem.setShader(shader);

	for (int i = 0; i < QTD_STICKERS; i++)
	{
		Object obj;
		obj.initialize();
		obj.setShader(shader);
		obj.setPosition(glm::vec3(-0.9 + espacoEntreStickers * i, 0.72, 1.0));
		obj.setDimensions(glm::vec3(0.138, 0.138, 1.0));

		stickers.push_back(obj);
	}

		definirTexturaStickers();
}

void solicitarArquivoImagem()
{
	char caminhoArquivoImagem[400];

	printf("Informe o caminho da imagem que você deseja editar: ");
	gets_s(caminhoArquivoImagem);

	arquivoInformado = true;
	imagem.setTexture(caminhoArquivoImagem);
	imagem.setDimensions(glm::vec3(2.0, 2.0, 1.0));
}

void mostrarTelaInicial()
{
	telaInicial.update();
	telaInicial.draw();
}

void definirTexturaStickers()
{
	stickers[0].setTexture("./textures/stickers/christmas-hat.png");
	stickers[1].setTexture("./textures/stickers/flower.png");
	stickers[2].setTexture("./textures/stickers/hang-loose.png");
	stickers[3].setTexture("./textures/stickers/happy-ghost.png");
	stickers[4].setTexture("./textures/stickers/happy-people.png");
	stickers[5].setTexture("./textures/stickers/joystick.png");
	stickers[6].setTexture("./textures/stickers/love.png");
	stickers[7].setTexture("./textures/stickers/pizza.png");
	stickers[8].setTexture("./textures/stickers/sleep.png");
	stickers[9].setTexture("./textures/stickers/sports.png");
	stickers[10].setTexture("./textures/stickers/vacation.png");
}