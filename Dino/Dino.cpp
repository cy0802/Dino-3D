#include "Dino.h"
Dino::Dino() {
	std::vector<float> v = ObjReader::read((char*)"..\\ObjReader\\Dino.obj");
	float _scale = *(v.end() - 1);
	if (*(v.end() - 2) > _scale) _scale = *(v.end() - 2);
	if (*(v.end() - 3) > _scale) _scale = *(v.end() - 2);
	this->scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0/_scale, 1.0/_scale, 1.0/_scale));
	std::copy(v.begin(), v.end(), data);
	rotationX = rotationY = rotationZ = glm::mat4(1.0f);
}
void Dino::rotate(float angle, char axis) {
	if (axis == 'x') {
		rotationX = rotationX * glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
	} else if (axis == 'y') {
		rotationY = rotationY * glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	} else {
		rotationZ = rotationZ * glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
	}
	rotation = rotationX * rotationY * rotationZ;
}