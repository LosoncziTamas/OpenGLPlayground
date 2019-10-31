#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "utils/utils.h"

// Try to draw 2 triangles next to each other using glDrawArrays by adding more vertices to your data.

typedef GLuint Shader;
typedef GLuint Program;
typedef GLuint VAO;
typedef GLuint VBO;

void FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

VBO CreateVBO()
{
    float vertices[] =
    {
           -0.5f, -0.5f, 0.0f, // bottom-left
            0.5f, -0.5f, 0.0f, // bottom-right
            0.0,   0.5f, 0.0f, // center-top
    };

    VBO result;
    glGenBuffers(1, &result);
    glBindBuffer(GL_ARRAY_BUFFER, result);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    return result;
}

VAO CreateVAO()
{
    VAO result;
    glGenVertexArrays(1, &result);
    glBindVertexArray(result);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
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

int main()
{
    if (glfwInit())
    {
        GLFWwindow* window = CreateWindow("Exercise 1");

        if (window && gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            glfwMakeContextCurrent(window);
            glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);

            const char* vertexShaderSource = ReadTextFile("glfw-ex1/ex1.vert");
            const char* fragmentShaderSource = ReadTextFile("glfw-ex1/ex1.frag");

            if (fragmentShaderSource && vertexShaderSource)
            {
                Shader vertex = CreateShader(GL_VERTEX_SHADER, vertexShaderSource);
                CheckShaderState(vertex, window);
                Shader fragment = CreateShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
                CheckShaderState(fragment, window);
                Program program = CreateShaderProgram(vertex, fragment);
                CheckProgramState(program, window);

                glUseProgram(program);

                VBO vbo = CreateVBO();
                VAO vao = CreateVAO();

                while (!glfwWindowShouldClose(window))
                {
                    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                    {
                        glfwSetWindowShouldClose(window, GLFW_TRUE);
                    }

                    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                    glClear(GL_COLOR_BUFFER_BIT);
                    glDrawArrays(GL_TRIANGLES, 0, 3);

                    glfwSwapBuffers(window);
                    glfwPollEvents();
                }

                glDeleteBuffers(1, &vbo);
                glDeleteVertexArrays(1, &vao);
                glDeleteProgram(program);
                glDeleteShader(vertex);
                glDeleteShader(fragment);
            }
            glfwDestroyWindow(window);
        }
    }
}