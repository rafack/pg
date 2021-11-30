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

vector <Shader*> shaders;
Shader* shader;
#define SHADER_FILTRO_BINARIZACAO "./shaders/fragment_shader-binarizacao.fs"
#define SHADER_FILTRO_COLORIZACAO "./shaders/fragment_shader-colorizacao.fs"
#define SHADER_FILTRO_GRAYSCALE "./shaders/fragment_shader-grayscale.fs"
#define SHADER_FILTRO_INVERSAO "./shaders/fragment_shader-inversao.fs"
#define SHADER_FILTRO_MEUFILTRO "./shaders/fragment_shader-meufiltro.fs"
#define SHADER_FILTRO_RGB "./shaders/fragment_shader-rgb.fs"
#define SHADER_FILTRO_VIGNETE "./shaders/fragment_shader-vignete.fs"

float espacoEntreFiltros = 0.09;

Object telaInicial;
Object barraSuperior;
Object barraInferior;
Object imagem;

vector <Object> stickers;
vector <Object> stickersEmUso;
vector <Object> stickerSelecionado;
vector <Object> previasFiltros;

const float METADE_STICKER_TAMANHO1_PIXELS = 48.0;
const float METADE_STICKER_TAMANHO2_PIXELS = 58.0;
const float METADE_STICKER_TAMANHO3_PIXELS = 60.0;
const float METADE_STICKER_TAMANHO4_PIXELS = 64.0;

const int QTD_STICKERS = 11;
float espacoEntreStickers = 0.09;

char caminhoArquivoImagem[400];
bool arquivoInformado;
bool isStickerSelecionado;

double xCursor, yCursor;

int main();

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

void carregarGraficos();
void iniciarObjetos();
void solicitarArquivoImagem();
void mostrarTelaInicial();
void definirStickers();
void criarShaders();

Shader* getShaderByIdx(int i);

enum cores { R, G, B };
void aplicarFiltro(int cor);

int main()
{
	carregarGraficos();

	criarShaders();
	shader = new Shader("./shaders/fragment_shader.fs");
	shader->use();
	shader->setInt("cor", -1);
	shader->setVec4("corColorizadora", 0.675, 0.341, 0.501);
	shader->setVec2("dimensoesTela", WIDTH, HEIGHT);

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
		projection = glm::ortho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
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

			barraInferior.update();
			barraInferior.draw();

			for (int i = 0; i < stickers.size(); i++)
			{
				stickers[i].update();
				stickers[i].draw();
			}

			for(Object sticker : stickersEmUso)
			{
				sticker.update();
				sticker.draw();
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

	if (key == GLFW_KEY_R && action == GLFW_PRESS)
		aplicarFiltro(R);

	if (key == GLFW_KEY_G && action == GLFW_PRESS)
		aplicarFiltro(G);

	if (key == GLFW_KEY_B && action == GLFW_PRESS)
		aplicarFiltro(B);

	if (key == GLFW_KEY_N && action == GLFW_PRESS)
		shader->setInt("cor", -1);

	if (isStickerSelecionado)
	{
		if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
		{
			Object & sticker = stickersEmUso.back();
			if (sticker.getIsSelecionado()) 
			{
				sticker.moverParaEsquerda();
			}
		}

		if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
		{
			Object& sticker = stickersEmUso.back();
			if (sticker.getIsSelecionado())
			{
				sticker.moverParaDireita();
			}
		}

		if (key == GLFW_KEY_UP && action == GLFW_PRESS)
		{
			Object& sticker = stickersEmUso.back();
			if (sticker.getIsSelecionado())
			{
				sticker.moverParaCima();
			}
		}

		if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
		{
			Object& sticker = stickersEmUso.back();
			if (sticker.getIsSelecionado())
			{
				sticker.moverParaBaixo();
			}
		}
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		glfwGetCursorPos(window, &xCursor, &yCursor);

		for (int i = 0; i < 7; i++)
		{
			if (xCursor >= 11.7 + (i * 145) && xCursor <= 87.2 + 75.5 + (i * 145))
			{
				if (720.0 - yCursor <= 639.0)
				{
					imagem.setShader(getShaderByIdx(i));
					imagem.update();
					imagem.draw();
					printf("nome do filtro %s", getShaderByIdx(i)->getNome().c_str());
				}
			}
		}

		for (Object sticker : stickers)
		{
			if (xCursor >= (sticker.getPosX() * 1280 - sticker.getMetadeTamanho())
				&& xCursor <= (sticker.getPosX() * 1280 + sticker.getMetadeTamanho()))
			{
				if (720.0 - yCursor >= (sticker.getPosY() * 720 - sticker.getMetadeTamanho())
					&& 720.0 - yCursor <= (sticker.getPosY() * 720 + sticker.getMetadeTamanho()))
				{
					Object obj;
					obj.initialize();
					obj.setShader(shader);
					obj.setPosition(glm::vec3(0.5, 0.5, 1.0));
					obj.setDimensions(glm::vec3(0.07, 0.07, 1.0));
					obj.setTexture(sticker.getNome());
					obj.setMetadeTamanhoEmPixels(sticker.getMetadeTamanho());

					obj.setIsSelecionado(true);
					isStickerSelecionado = true;

					stickersEmUso.push_back(obj);
				}
			}
		}

		for (Object sticker : stickersEmUso)
		{
			if (xCursor >= (sticker.getPosX() * 1280 - sticker.getMetadeTamanho())
				&& xCursor <= (sticker.getPosX() * 1280 + sticker.getMetadeTamanho()))
			{
				if (720.0 - yCursor >= (sticker.getPosY() * 720 - sticker.getMetadeTamanho())
					&& 720.0 - yCursor <= (sticker.getPosY() * 720 + sticker.getMetadeTamanho()))
				{
					sticker.setIsSelecionado(true);
					isStickerSelecionado = true;
				}
			}
		}
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
	
	barraSuperior.initialize();
	barraSuperior.setShader(shader);
	barraSuperior.setTexture("./textures/barra-superior.png");
	barraSuperior.setPosition(glm::vec3(0.3, 0.95, 1.0));
	barraSuperior.setDimensions(glm::vec3(1.4, 0.16, 1.0));

	barraInferior.initialize();
	barraInferior.setShader(shader);
	barraInferior.setTexture("./textures/barra-inferior.png");
	barraInferior.setPosition(glm::vec3(0.558, 0.06, 1.0));
	barraInferior.setDimensions(glm::vec3(1.13, 0.18, 1.0));
	
	imagem.initialize();
	imagem.setShader(shader);

	for (int i = 0; i < QTD_STICKERS; i++)
	{
		Object obj;
		obj.initialize();
		obj.setShader(shader);
		obj.setPosition(glm::vec3(0.05 + espacoEntreStickers * i, 0.931, 1.0));
		obj.setDimensions(glm::vec3(0.07, 0.07, 1.0));

		stickers.push_back(obj);
	}

	for (int i = 0; i < 7; i++)
	{
		Shader* shaderInversao = new Shader(SHADER_FILTRO_INVERSAO);

		Object obj;
		obj.initialize();
		obj.setShader(shader);
		obj.setPosition(glm::vec3(0.05 + espacoEntreFiltros * i, 0.5, 1.0));
		obj.setDimensions(glm::vec3(0.07, 0.1, 1.0));
		obj.setTexture("./textures/tela-inicial.png");

		previasFiltros.push_back(obj);
	}

		definirStickers();
}

void solicitarArquivoImagem()
{
	printf("Digite o caminho completo da imagem: ");
	gets_s(caminhoArquivoImagem);

	arquivoInformado = true;
	imagem.setTexture(caminhoArquivoImagem);
}

void mostrarTelaInicial()
{
	telaInicial.update();
	telaInicial.draw();
}

void definirStickers()
{
	stickers[0].setTexture("./textures/stickers/christmas-hat.png");
	stickers[0].setTextureName("./textures/stickers/christmas-hat.png");
	stickers[0].setMetadeTamanhoEmPixels(METADE_STICKER_TAMANHO4_PIXELS);

	stickers[1].setTexture("./textures/stickers/flower.png");
	stickers[1].setTextureName("./textures/stickers/flower.png");
	stickers[1].setMetadeTamanhoEmPixels(METADE_STICKER_TAMANHO3_PIXELS);

	stickers[2].setTexture("./textures/stickers/hang-loose.png");
	stickers[2].setTextureName("./textures/stickers/hang-loose.png");
	stickers[2].setMetadeTamanhoEmPixels(METADE_STICKER_TAMANHO2_PIXELS);

	stickers[3].setTexture("./textures/stickers/happy-ghost.png");
	stickers[3].setTextureName("./textures/stickers/happy-ghost.png");
	stickers[3].setMetadeTamanhoEmPixels(METADE_STICKER_TAMANHO4_PIXELS);

	stickers[4].setTexture("./textures/stickers/happy-people.png");
	stickers[4].setTextureName("./textures/stickers/happy-people.png");
	stickers[4].setMetadeTamanhoEmPixels(METADE_STICKER_TAMANHO4_PIXELS);

	stickers[5].setTexture("./textures/stickers/joystick.png");
	stickers[5].setTextureName("./textures/stickers/joystick.png");
	stickers[5].setMetadeTamanhoEmPixels(METADE_STICKER_TAMANHO1_PIXELS);

	stickers[6].setTexture("./textures/stickers/love.png");
	stickers[6].setTextureName("./textures/stickers/love.png");
	stickers[6].setMetadeTamanhoEmPixels(METADE_STICKER_TAMANHO3_PIXELS);

	stickers[7].setTexture("./textures/stickers/pizza.png");
	stickers[7].setTextureName("./textures/stickers/pizza.png");
	stickers[7].setMetadeTamanhoEmPixels(METADE_STICKER_TAMANHO4_PIXELS);

	stickers[8].setTexture("./textures/stickers/sleep.png");
	stickers[8].setTextureName("./textures/stickers/sleep.png");
	stickers[8].setMetadeTamanhoEmPixels(METADE_STICKER_TAMANHO4_PIXELS);

	stickers[9].setTexture("./textures/stickers/sports.png");
	stickers[9].setTextureName("./textures/stickers/sports.png");
	stickers[9].setMetadeTamanhoEmPixels(METADE_STICKER_TAMANHO4_PIXELS);

	stickers[10].setTexture("./textures/stickers/vacation.png");
	stickers[10].setTextureName("./textures/stickers/vacation.png");
	stickers[10].setMetadeTamanhoEmPixels(METADE_STICKER_TAMANHO4_PIXELS);
}

void aplicarFiltro(int cor)
{
	imagem.getShader()->setInt("cor", cor);
}

void criarShaders()
{
	Shader* shaderBinarizacao = new Shader(SHADER_FILTRO_BINARIZACAO);
	shaderBinarizacao->setNome("Binarização");
	shaders.push_back(shaderBinarizacao);
	
	Shader* shaderColor = new Shader(SHADER_FILTRO_COLORIZACAO);
	shaderColor->setNome("Colorização");
	shaders.push_back(shaderColor);

	Shader* shaderInversao = new Shader(SHADER_FILTRO_INVERSAO);
	shaderInversao->setNome("Inversão");
	shaders.push_back(shaderInversao);

	Shader* rgb = new Shader(SHADER_FILTRO_RGB);
	rgb->setNome("RGB");
	shaders.push_back(rgb);

	Shader* gray = new Shader(SHADER_FILTRO_GRAYSCALE);
	gray->setNome("Grayscale");
	shaders.push_back(gray);

	Shader* meufiltro = new Shader(SHADER_FILTRO_MEUFILTRO);
	meufiltro->setNome("MeuFiltro");
	shaders.push_back(meufiltro);
	
	Shader* vignete = new Shader(SHADER_FILTRO_VIGNETE);
	vignete->setNome("Vignete");
	shaders.push_back(vignete);
}

Shader* getShaderByIdx(int i) 
{
	Shader* shaderInversao = new Shader(SHADER_FILTRO_INVERSAO);
	Shader* shaderBinarizacao = new Shader(SHADER_FILTRO_BINARIZACAO);
	Shader* shaderRGB = new Shader(SHADER_FILTRO_RGB);
	Shader* shaderColorizacao = new Shader(SHADER_FILTRO_COLORIZACAO);
	Shader* shaderMeuFiltro = new Shader(SHADER_FILTRO_MEUFILTRO);
	Shader* shaderGrayscale = new Shader(SHADER_FILTRO_GRAYSCALE);
	Shader* shaderVignete = new Shader(SHADER_FILTRO_VIGNETE);

	switch (i) {
	case 0:
		return shaderInversao;
		break;
	case 1:
		return shaderBinarizacao;
		break;
	case 2:
		return shaderColorizacao;
		break;
	case 3:
		return shaderVignete;
		break;
	case 4:
		return shaderGrayscale;
		break;
	case 5:
		return shaderMeuFiltro;
		break;
	case 6:
		return shaderRGB;
		break;
	}
}