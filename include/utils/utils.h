#ifndef UTILS_H
#define UTILS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

GLFWwindow* Utils_CreateWindow(const char* title);
void Utils_CheckShaderState(GLuint shader, GLFWwindow* window);
void Utils_CheckProgramState(GLuint program, GLFWwindow* window);
char* Utils_ReadTextFile(const char* fileName);

#endif