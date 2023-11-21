#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <windows.h>
#define STB_IMAGE_IMPLEMENTATION
#include "Includes/stb_image.h"
#include "Shader/Shader.h"
#include "ObjReader/ObjReader.h" 
#include "Dino/Dino.h"
#include "Light/Light.h"


const unsigned int SCR_WIDTH = 1300;
const unsigned int SCR_HEIGHT = 1300;
GLFWwindow* window;
void init();
void processInput(GLFWwindow* window);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
    GLsizei length, const GLchar* message, const void* userParam);
Dino dino((char*)"..\\resource\\tiger.obj", (char*)"..\\resource\\tiger.mtl",
    (char*)"..\\resource\\dino.vs", (char*)"..\\resource\\dino.fs", (char*)"..\\resource\\tiger-altas.jpg");
Light light0(glm::vec3(1.0f, 1.0f, 3.0f));
Light light1(glm::vec3(-1.0f, -1.0f, 3.0f));
glm::vec3 camera = glm::vec3(0.0, 0.0, 0.0);

// for mouse input
float lastX = 0.0f;
float lastY = 0.0f;
float vertical = 0.0f;
float horizontal = 0.0f;

int main() {
    init();
    Shader shader((char*)"..\\Dino\\dino.vs", (char*)"..\\Dino\\dino.fs");
    light1.color = glm::vec3(0.827451, 0.788235, 0.705882);

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(dino.data), dino.data, GL_STATIC_DRAW);

    // position attribute: vertex and normal
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(2);

    unsigned int texture = dino.loadTexture();
    //glGenTextures(1, &texture);
    //glBindTexture(GL_TEXTURE_2D, texture);
    //// set the texture wrapping parameters
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //// set texture filtering parameters
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //// load image, create texture and generate mipmaps
    //int width, height, nrChannels;
    //stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    //// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    //unsigned char* textureImg = stbi_load("..\\resource\\tiger-atlas.jpg", &width, &height, &nrChannels, 0);
    //if (textureImg) {
    //    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureImg);
    //    glGenerateMipmap(GL_TEXTURE_2D);
    //    std::cout << "successfully load texture\nheight = " << height
    //        << ", width = " << width << '\n';
    //} else {
    //    std::cout << "Failed to load texture" << std::endl;
    //    std::cout << stbi_failure_reason() << std::endl;
    //}
    //stbi_image_free(textureImg);

    glm::mat4 projection = glm::ortho(0.7, -0.7, 0.7, -0.7, -1.5, 1.5);
    glm::mat4 model = glm::mat4(1.0f);

    float t = 0.0;

    dino.rotate(90.0, 'z');
    dino.rotate(90.0, 'x');

    shader.use();
    shader.setInt((char*)"texture_", 0);
    shader.setMat4((char*)"projection", projection);
    shader.setMat4((char*)"model", model);

    shader.setVec3((char*)"objectColor", dino.color);
    shader.setMat4((char*)"local", dino.rotation * dino.scale);
    shader.setVec3((char*)"viewPos", camera);

    shader.setVec3((char*)"light[0].position", light0.position);
    shader.setVec3((char*)"light[1].position", light1.position);
    shader.setVec3((char*)"light[0].color", light0.color);
    shader.setVec3((char*)"light[1].color", light1.color);
    shader.setVec3((char*)"light[0].ambient", glm::vec3(dino.ambient, dino.ambient, dino.ambient) * light0.color);
    shader.setVec3((char*)"light[1].ambient", glm::vec3(dino.ambient, dino.ambient, dino.ambient) * light1.color);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.8, 0.8, 0.8, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        glm::vec3 viewpos = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::mat4 view = glm::lookAt(camera, viewpos, up);
        shader.setMat4((char*)"view", view * translate);
        shader.setVec3((char*)"viewPos", camera);
        t += 0.2;
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 60000);
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            std::cout << "OpenGL Error: " << error << std::endl;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
        Sleep(30);
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void init() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "3D Dinosaur", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
    glDebugMessageCallback(MessageCallback, 0);
    glEnable(GL_DEPTH_TEST);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    float xOffset = lastX - xpos;
    float yOffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.04f;
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    horizontal += xOffset;
    vertical += yOffset;

    // camera = glm::vec3(cos(glm::radians(horizontal), cos))
    glm::vec3 direction;
    direction.x = cos(glm::radians(horizontal)) * cos(glm::radians(vertical));
    direction.y = sin(glm::radians(vertical));
    direction.z = sin(glm::radians(horizontal)) * cos(glm::radians(vertical));
    camera = glm::normalize(direction);
}

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id,
    GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    std::cerr << "GL CALLBACK: " << message << std::endl;
}