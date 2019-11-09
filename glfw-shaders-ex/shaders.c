#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "utils/utils.h"

#include <stdio.h>
#include <stdlib.h>

#define Assert(condition, text) if (!(condition)) { \
    printf("[ASSERTION FAILED]: %s Line %d in %s, %s.\n ", (text), __LINE__, __FILE__, __func__);  \
    CleanUp(); }

static GLFWwindow* window;

void CleanUp()
{
    if (window)
    {
        glfwDestroyWindow(window);
    }
    glfwTerminate();
    exit(1);
}

void KeyPressed(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void ResizeFrameBuffer(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// ex-1: Adjust the vertex shader so that the triangle is upside down
int main()
{
    Assert(glfwInit(), "GLFW init failure.");
    window = Utils_CreateWindow("Shaders - ex 1");
    Assert(window, "Window creation failure");
    Assert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "GLAD load failure");

    glfwSetKeyCallback(window, KeyPressed);
    glfwSetFramebufferSizeCallback(window, ResizeFrameBuffer);

    const char* vertSrc = Utils_ReadTextFile("upside_down.vert");
    const char* fragSrc = Utils_ReadTextFile("orange.frag");
    Assert(vertSrc && fragSrc, "Shader loading error.");

    GLuint vertShdr = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShdr, 1, &vertSrc, NULL);
    glCompileShader(vertShdr);
    Utils_CheckShaderState(vertShdr, window);

    GLuint fragShdr = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShdr, 1, &fragSrc, NULL);
    glCompileShader(fragShdr);
    Utils_CheckShaderState(fragShdr, window);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertShdr);
    glAttachShader(program, fragShdr);
    glLinkProgram(program);
    Utils_CheckProgramState(program, window);
    glUseProgram(program);

    float vertices[] = 
    {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    GLuint vertexBuff;
    glGenBuffers(1, &vertexBuff);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuff);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint vertexArr;
    glGenVertexArrays(1, &vertexArr);
    glBindVertexArray(vertexArr);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    CleanUp();
}