#pragma once
#include <glad/glad.h>
#include <fstream>
#include <errno.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../Shader/Shader.h"
#include "../Light/Light.h"
#include "../ObjReader/ObjReader.h"
class Object {
public:
	float data[50000000]; // vertex, normal
	glm::mat4 rotationX, rotationY, rotationZ, rotation;
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f));
	glm::vec3 color = glm::vec3(0.827451, 0.788235, 0.705882);
	glm::mat4 projection = glm::ortho(0.7, -0.7, 0.7, -0.7, -1.5, 1.5);
	// glm::mat4 projection = glm::ortho(100.0, -100.0, 100.0, -100.0, -200.0, 200.0);
	glm::mat4 model = glm::mat4(1.0f);
	glm::vec3 ambient, diffuse, specular;
	float opticalDensity;
	Shader shader;
	unsigned int texture = -1;
	char* texturePath;
	// float ambient = 0.1f;
	
	unsigned int loadTexture();
	void readMtl(char* mtlPath);
	Object(char* objfile, char* mtlfile, char* textureImg);
	void rotate(float angle, char axis);
	void setup(Light light, glm::vec3 camera);
};