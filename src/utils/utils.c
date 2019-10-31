#include "utils/utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

GLFWwindow* CreateWindow(const char* title)
{
    GLFWwindow* result = glfwCreateWindow(800, 600, title, NULL, NULL);

    if (result)
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
        glfwMakeContextCurrent(result);
        glfwSwapInterval(1);
    }

    return result;
}

char* ReadTextFile(const char* fileName)
{
    FILE *file = fopen(fileName, "r");
    if (!file)
    {
        return NULL;
    }

    if (fseek(file, 0, SEEK_END))
    {
        fclose(file);
        return NULL;
    }

    int elementCount = ftell(file);
    rewind(file);
    char* result = calloc(elementCount + 1, 1);
    assert(result);

    if (elementCount != fread(result, 1, elementCount, file))
    {
        free(result);
        result = NULL;
    }
    
    fclose(file);

    return result;
}

void CheckShaderState(GLuint shader, GLFWwindow* window)
{
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        int logLen;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
        char* infoLog = alloca(logLen);
        
        glGetShaderInfoLog(shader, logLen, NULL, infoLog);
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        fprintf(stderr, "Shader compilation error: %s\n", infoLog);
        getchar();
    }
}

void CheckProgramState(GLuint program, GLFWwindow* window)
{
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        int logLen;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
        char* infoLog = alloca(logLen);

        glGetProgramInfoLog(program, logLen, NULL, infoLog);
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        fprintf(stderr, "Shader program error: %s\n", infoLog);
        getchar();
    }
}

