#ifndef UTILS_H
#define UTILS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

GLFWwindow* CreateWindow(const char* title);
void CheckShaderState(GLuint shader, GLFWwindow* window);
void CheckProgramState(GLuint program, GLFWwindow* window);
char* ReadTextFile(const char* fileName);

#endif