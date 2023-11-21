#pragma once
#include "Object.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../Includes/stb_image.h"


Object::Object(char* objfile, char* mtlfile, char* textureImg) {
	std::vector<float> v = ObjReader::read(objfile);
	float _scale = *(v.end() - 1);
	if (*(v.end() - 2) > _scale) _scale = *(v.end() - 2);
	if (*(v.end() - 3) > _scale) _scale = *(v.end() - 2);
	this->scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0/_scale, 1.0/_scale, 1.0/_scale));
	std::copy(v.begin(), v.end(), data);
	rotationX = rotationY = rotationZ = glm::mat4(1.0f);
}
void Object::setup(Light light, glm::vec3 camera) {
    this->shader.use();
    this->shader.setInt((char*)"texture_", 0);
    this->shader.setMat4((char*)"projection", this->projection);
    this->shader.setMat4((char*)"model", this->model);

    this->shader.setVec3((char*)"objectColor", this->color);
    this->shader.setMat4((char*)"local", this->rotation * this->scale);
    this->shader.setVec3((char*)"viewPos", camera);

    this->shader.setVec3((char*)"light[0].position", light.position);
    this->shader.setVec3((char*)"light[0].color", light.color);
    this->shader.setVec3((char*)"light[0].ambient", 
        glm::vec3(this->ambient, this->ambient, this->ambient) * light.color);
    this->shader.setVec3((char*)"light[1].position", light.position);
    this->shader.setVec3((char*)"light[1].color", light.color);
    this->shader.setVec3((char*)"light[1].ambient",
        glm::vec3(this->ambient, this->ambient, this->ambient) * light.color);
}
unsigned int Object::loadTexture() {
    glGenTextures(1, &this->texture);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char* textureImg = stbi_load("..\\resource\\tiger-atlas.jpg", &width, &height, &nrChannels, 0);
    if (textureImg) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureImg);
        glGenerateMipmap(GL_TEXTURE_2D);
        std::cout << "successfully load texture\nheight = " << height
            << ", width = " << width << '\n';
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
        std::cout << stbi_failure_reason() << std::endl;
    }
    stbi_image_free(textureImg);
    return this->texture;
}
void Object::rotate(float angle, char axis) {
	if (axis == 'x') {
		rotationX = rotationX * glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
	} else if (axis == 'y') {
		rotationY = rotationY * glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	} else {
		rotationZ = rotationZ * glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
	}
	rotation = rotationX * rotationY * rotationZ;
}