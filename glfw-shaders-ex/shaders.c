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
void FirstSolution()
{
    Assert(glfwInit(), "GLFW init failure.");
    window = Utils_CreateWindow("Shader exercise 1");
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

    glDeleteShader(vertShdr);
    glDeleteShader(fragShdr);
    free((void*)fragSrc);
    free((void*)vertSrc);

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

// ex-2: Specify a horizontal offset via a uniform and move the triangle to the right side of the screen in the vertex shader using this offset value.
void SecondSolution()
{
    Assert(glfwInit(), "GLFW init failure.");
    window = Utils_CreateWindow("Shader exercise 1");
    Assert(window, "Window creation failure");
    Assert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "GLAD load failure");

    glfwSetKeyCallback(window, KeyPressed);
    glfwSetFramebufferSizeCallback(window, ResizeFrameBuffer);

    const char* vertSrc = Utils_ReadTextFile("offset.vert");
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

    glDeleteShader(vertShdr);
    glDeleteShader(fragShdr);
    free((void*)fragSrc);
    free((void*)vertSrc);

    int offsetParamLocation = glGetUniformLocation(program, "horizontalOffset");
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

    float offset = 0.0f;
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        {
            offset += 0.01f;
            glUniform1f(offsetParamLocation, offset);
        }
        else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        {
            offset -= 0.01f;
            glUniform1f(offsetParamLocation, offset);
        }

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    CleanUp();
}

// Output the vertex position to the fragment shader using the out keyword and set the fragment's color equal to this vertex position. 
// Once you managed to do this; try to answer the following question: why is the bottom-left side of our triangle black?
void ThirdSolution()
{
    Assert(glfwInit(), "GLFW init failure.");
    window = Utils_CreateWindow("Shader exercise 1");
    Assert(window, "Window creation failure");
    Assert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "GLAD load failure");

    glfwSetKeyCallback(window, KeyPressed);
    glfwSetFramebufferSizeCallback(window, ResizeFrameBuffer);

    const char* vertSrc = Utils_ReadTextFile("default.vert");
    const char* fragSrc = Utils_ReadTextFile("colorPos.frag");
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

    glDeleteShader(vertShdr);
    glDeleteShader(fragShdr);
    free((void*)fragSrc);
    free((void*)vertSrc);

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

int main()
{
    ThirdSolution();
}