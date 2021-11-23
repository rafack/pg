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

using namespace std;

const GLuint WIDTH = 1100, HEIGHT = 700;

GLFWwindow* window;

Shader* shader;

Object background;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

void carregarGraficos();
void inicializarObjetos();
void aplicarFiltro(int cor);

enum cores { R, G, B };

int main()
{
	carregarGraficos();

	shader = new Shader();
	shader->use();
	shader->setInt("cor", -1);
	shader->setVec4("corColorizadora", 0.675, 0.341, 0.501);

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

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_R && action == GLFW_PRESS)
		aplicarFiltro(R);

	if (key == GLFW_KEY_G && action == GLFW_PRESS)
		aplicarFiltro(G);

	if (key == GLFW_KEY_B && action == GLFW_PRESS)
		aplicarFiltro(B);

	if (key == GLFW_KEY_N && action == GLFW_PRESS)
		shader->setInt("cor", -1);
}

void carregarGraficos()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Ola, Lista 5!", nullptr, nullptr);
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
	background.setTexture("./textures/beach-bg.png");
	background.setDimensions(glm::vec3(2.0, 2.0, 0.0));
}

void aplicarFiltro(int cor)
{
	shader->setInt("cor", cor);
}
