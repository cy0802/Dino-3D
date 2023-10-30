#include "Light.h"
Light::Light(glm::vec3 position, glm::vec3 constants) {
	this->position = position;
	this->constants = constants;
}