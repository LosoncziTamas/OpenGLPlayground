#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "utils/utils.h"
#include <stdio.h>
#include <stdlib.h>

// 1. Try to draw 2 triangles next to each other using glDrawArrays by adding more vertices to your data.
// 2. Now create the same 2 triangles using two different VAOs and VBOs for their data.


typedef GLuint Shader;
typedef GLuint Program;
typedef GLuint VAO;
typedef GLuint VBO;
typedef GLuint EBO;

void FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

EBO CreateEBO(unsigned int *indices, unsigned int indexCount)
{
    EBO result;
    glGenBuffers(1, &result);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, result);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);
    return result;
}

VBO CreateVBO(float* vertices, unsigned int vertexCount)
{
    VBO result;
    glGenBuffers(1, &result);
    glBindBuffer(GL_ARRAY_BUFFER, result);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float), vertices, GL_STATIC_DRAW);
    return result;
}

VAO CreateVAO()
{
    VAO result;
    glGenVertexArrays(1, &result);
    glBindVertexArray(result);
    return result;
}

Shader CreateShader(GLenum shaderType, const char* source)
{
    Shader result = glCreateShader(shaderType);
    glShaderSource(result, 1, &source, NULL);
    glCompileShader(result);
    return result;
}

Shader CreateShaderProgram(Shader vertex, Shader fragment)
{
    Program program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    return program;
}

void _SafeFree(void* memory)
{
    if (memory)
    {
        free(memory);
    }
}
#define SafeFree(memory) _SafeFree((void*) (memory));

int main()
{
    if (!glfwInit())
    {
        return -1;
    }

    GLFWwindow* window = Utils_CreateWindow("GLFW - exercise 1");

    if (window)
    {   
        glfwSetKeyCallback(window, KeyCallback);
        glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);  

        if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            //TODO: switch to makefiles
            const char* vertexShaderSource = Utils_ReadTextFile("ex1.vert");
            const char* fragmentShaderSource = Utils_ReadTextFile("ex1.frag");

            if (fragmentShaderSource && vertexShaderSource)
            {
                float vertices[] =
                {
                       -1.0f, -0.5f, 0.0f, 
                        0.0f, -0.5f, 0.0f,    
                       -0.5,   0.5f, 0.0f,  
                        1.0f, -0.5f, 0.0f,  
                        0.5f,  0.5f, 0.0f
                };

                unsigned int indices[] =
                {
                    0, 1, 2,
                    1, 3, 4
                };

                VBO vbo = CreateVBO(vertices, ArraySize(vertices));

                VAO vao = CreateVAO();
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
                glEnableVertexAttribArray(0);

                EBO ebo = CreateEBO(indices, ArraySize(indices));

                Shader vertex = CreateShader(GL_VERTEX_SHADER, vertexShaderSource);
                Utils_CheckShaderState(vertex, window);
                Shader fragment = CreateShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
                Utils_CheckShaderState(fragment, window);
                Program program = CreateShaderProgram(vertex, fragment);
                Utils_CheckProgramState(program, window);
                glUseProgram(program);

                while (!glfwWindowShouldClose(window))
                {
                    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                    glClear(GL_COLOR_BUFFER_BIT);
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                    glfwSwapBuffers(window);
                    glfwPollEvents();
                }

                glDeleteBuffers(1, &ebo);
                glDeleteBuffers(1, &vbo);
                glDeleteVertexArrays(1, &vao);
                glDeleteProgram(program);
                glDeleteShader(vertex);
                glDeleteShader(fragment);

                SafeFree(vertexShaderSource);
                SafeFree(fragmentShaderSource);
            }
            glfwDestroyWindow(window);
        }
    }
}