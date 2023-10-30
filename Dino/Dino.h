#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../Shader/Shader.h"
#include "../ObjReader/ObjReader.h"
class Dino {
public:
	float data[500000]; // vertex, normal
	glm::mat4 rotationX, rotationY, rotationZ, rotation;
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f));
	glm::vec3 color = glm::vec3(0.827451, 0.788235, 0.705882);
	
	float ambient = 0.1f;
	
	Dino();
	void rotate(float angle, char axis);
};