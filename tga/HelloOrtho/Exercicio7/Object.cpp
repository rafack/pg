#include <chrono>
#include <random>
#include <string>
#include "Object.h"
#include "stb_image.h"
using namespace std;

void Object::initialize()
{
	GLuint VBO, EBO;

	float vertices[] = {
		// positions          // colors          // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0, // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0  // top left 
	};

	unsigned int indices[] = {
	0, 1, 3, // first triangle
	1, 2, 3  // second triangle
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	model = glm::mat4(1);
	scale = glm::vec3(1.0, 1.0, 1.0);
	position = glm::vec3(0.0, 0.0, 0.0);
	angle = 0.0;
}

void Object::update()
{
	shader->use();

	setTranslation(position, true);
	setRotation(angle, glm::vec3(0.0, 0.0, 1.0));
	setScale(scale);

	shader->setMat4("model", glm::value_ptr(model));
	shader->setInt("tex1", 0);
}

void Object::draw()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Object::setTexture(string pathname)
{
	// Gera o identificador da textura na memória 
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	//Ajusta os parâmetros de wrapping e filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	//Carregamento da imagem
	int width, height, nrChannels;
	unsigned char* data = stbi_load(pathname.c_str(), &width, &height, &nrChannels, 0);	

	if (data)
	{
		if (nrChannels == 3) //jpg, bmp
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else //png
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Object::setRotation(float angle, glm::vec3 axis, bool reset)
{
	if (reset) 
	{
		model = glm::mat4(1);
	}
	model = glm::rotate(model, angle, axis);
}

void Object::setTranslation(glm::vec3 matrix, bool reset)
{
	if (reset)
	{
		model = glm::mat4(1);
	}
	model = glm::translate(model, matrix);
}

void Object::setScale(glm::vec3 matrix, bool reset)
{
	if (reset)
	{
		model = glm::mat4(1);
	}
	model = glm::scale(model, matrix);
}

void Object::moverParaDireita()
{
	setPosition(glm::vec3(this->getPosX() + 0.08, this->getPosY(), 0.0));
}

void Object::moverParaEsquerda()
{
	setPosition(glm::vec3(this->position.x - 0.08, this->position.y, 0.0));
}

void Object::posicaoAleatoriaEmX()
{
	double xMin = -0.8;
	double xMax = 0.8;
	std::uniform_real_distribution<double> unif(xMin, xMax);
	std::default_random_engine re;
	re.seed(std::chrono::system_clock::now().time_since_epoch().count());
	
	double randomPosX = unif(re);
	double topoJanela = 1.0;

	setPosition(glm::vec3(randomPosX, topoJanela, 0.0));
}