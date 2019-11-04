#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "utils/utils.h"
#include <stdio.h>

// Try to draw 2 triangles next to each other using glDrawArrays by adding more vertices to your data.

typedef GLuint Shader;
typedef GLuint Program;
typedef GLuint VAO;
typedef GLuint VBO;

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

const char* vertexShaderSource = 
"#version 330 core\n\
layout (location = 0) in vec3 aPos;\
\
void main()\
{\
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\
}";

const char* fragmentShaderSource = 
"#version 330 core\n\
out vec4 FragColor;\
\
void main()\
{\
    FragColor = vec4(1.0f, 0.5f, 0.2f, 0.75f);\
}";

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(640, 480, "GLFW - OpenGL", NULL, NULL);

    if (window)
    {   
        glfwSetKeyCallback(window, KeyCallback);
        glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);  
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);

        if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            //TODO: fix file loading issue
            //TODO: switch to makefiles
            if (fragmentShaderSource && vertexShaderSource)
            {
                Shader vertex = CreateShader(GL_VERTEX_SHADER, vertexShaderSource);
                Utils_CheckShaderState(vertex, window);
                Shader fragment = CreateShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
                Utils_CheckShaderState(fragment, window);
                Program program = CreateShaderProgram(vertex, fragment);
                Utils_CheckProgramState(program, window);

                glUseProgram(program);

                VBO vbo = CreateVBO();
                VAO vao = CreateVAO();

                while (!glfwWindowShouldClose(window))
                {
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