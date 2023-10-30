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
#include "Shader/Shader.h"
#include "ObjReader/ObjReader.h" 
#include "Dino/Dino.h"
#include "Light/Light.h"
#include "TestCube/TestCube.h"


const unsigned int SCR_WIDTH = 1300;
const unsigned int SCR_HEIGHT = 1300;
GLFWwindow* window;
void init();
void processInput(GLFWwindow* window);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
    GLsizei length, const GLchar* message, const void* userParam);
Dino dino;
Light light0(glm::vec3(1.0f, 1.0f, 3.0f), glm::vec3(1.0, 0.7, 1.8));
Light light1(glm::vec3(-1.0f, -1.0f, 3.0f), glm::vec3(1.0, 0.7, 1.8));
glm::vec3 camera = glm::vec3(0.0, 0.0, 0.0);

// for mouse input
float lastX = 0.0f;
float lastY = 0.0f;
float vertical = 0.0f;
float horizontal = 0.0f;

int main() {
    init();
    // TestCube testCube;
    Shader shader((char*)"..\\Dino\\dino.vs", (char*)"..\\Dino\\dino.fs");
    light1.color = glm::vec3(0.827451, 0.788235, 0.705882);

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(dino.data), dino.data, GL_STATIC_DRAW);

    // position attribute: vertex and normal
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);
    glm::mat4 projection = glm::ortho(0.7, -0.7, 0.7, -0.7, -1.5, 1.5);
    glm::mat4 model = glm::mat4(1.0f);

    float t = 0.0;

    dino.rotate(90.0, 'z');
    dino.rotate(90.0, 'x');

    shader.use();
    shader.setMat4((char*)"projection", projection);
    shader.setMat4((char*)"model", model);

    shader.setVec3((char*)"objectColor", dino.color);
    shader.setMat4((char*)"local", dino.rotation * dino.scale);
    shader.setVec3((char*)"viewPos", camera);

    shader.setVec3((char*)"light[0].position", light0.position);
    shader.setVec3((char*)"light[1].position", light1.position);
    shader.setVec3((char*)"light[0].color", light0.color);
    shader.setVec3((char*)"light[1].color", light1.color);
    shader.setVec3((char*)"light[0].constants", light0.constants);
    shader.setVec3((char*)"light[1].constants", light1.constants);
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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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

    float sensitivity = 0.05f;
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