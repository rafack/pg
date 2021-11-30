#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include "Shader.h"
using namespace std;

class Object
{
public:
	Object() {}
	~Object() {};
	void initialize();
	void update();
	void draw();
	void setTexture(string pathname);
	inline void setShader(Shader* shader) { this->shader = shader; }
	inline void setPosition(glm::vec3 position) { this->position = position; }
	inline float getPosX() { return this->position.x; }
	inline float getPosY() { return this->position.y; }
	inline void setDimensions(glm::vec3 scale) { this->scale = scale; }
	inline void setAngle(float angle) { this->angle = angle; }
	inline void setMetadeTamanhoEmPixels(float metadeTamanho) { this->metadeTamanhoEmPixels = metadeTamanho; }
	float getMetadeTamanho();
	void moverParaDireita();
	void moverParaEsquerda();
	void moverParaCima();
	void moverParaBaixo();
	void setNome(string nome) { this->nome = nome;  };
	string getNome() { return this->nome; };
	void setIsSelecionado(bool isSelecionado) { this->isSelecionado = isSelecionado;  };
	bool getIsSelecionado() { return this->isSelecionado;  };
	Shader* getShader() { return shader; }

protected:
	void setRotation(float angle, glm::vec3 axis, bool reset = false);
	void setTranslation(glm::vec3 matrix, bool reset = false);
	void setScale(glm::vec3 matrix, bool reset = false);
	glm::vec3 getScale();


	GLuint VAO;
	glm::mat4 model;
	GLuint texID;
	Shader* shader;
	glm::vec3 position, scale;
	float angle;
	float metadeTamanhoEmPixels; // usado para os stickers (quadrados)
	string nome;
	bool isSelecionado;
};
