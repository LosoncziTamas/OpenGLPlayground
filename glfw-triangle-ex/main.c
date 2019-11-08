#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "utils/utils.h"
#include <stdio.h>
#include <stdlib.h>

typedef GLuint Shader;
typedef GLuint Program;
typedef GLuint VAO;
typedef GLuint VBO;
typedef GLuint EBO;


// TODO: switch to makefiles
// TODO: release unused resources for ex.3

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


// 1. Try to draw 2 triangles next to each other using glDrawArrays by adding more vertices to your data.
void FirstSolution(GLFWwindow* window)
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
    glBindVertexArray(vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    EBO ebo = CreateEBO(indices, ArraySize(indices));

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
}

// 2. Now create the same 2 triangles using two different VAOs and VBOs for their data.
void SecondSolution(GLFWwindow* window)
{
    float verticesA[] =
    {
            -1.0f, -0.5f, 0.0f, 
             0.0f, -0.5f, 0.0f,    
            -0.5,   0.5f, 0.0f
    };
    VBO vboA = CreateVBO(verticesA, ArraySize(verticesA));

    VAO vaoA = CreateVAO();
    glBindVertexArray(vaoA);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    float verticesB[] =
    {
            0.0f, -0.5f, 0.0f,
            1.0f, -0.5f, 0.0f,  
            0.5f,  0.5f, 0.0f
    };

    VBO vboB = CreateVBO(verticesB, ArraySize(verticesB));

    VAO vaoB = CreateVAO();
    glBindVertexArray(vaoB);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glBindVertexArray(vaoA);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(vaoB);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &vboA);
    glDeleteBuffers(1, &vboB);
    glDeleteVertexArrays(1, &vaoA);
    glDeleteVertexArrays(1, &vaoB);
}

void FirstAndSecondSolutionSetup()
{
    if (!glfwInit())
    {
        exit(1);
    }

    GLFWwindow* window = Utils_CreateWindow("GLFW - exercise 1");

    if (window)
    {   
        glfwSetKeyCallback(window, KeyCallback);
        glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);  

        if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            const char* vertexShaderSource = Utils_ReadTextFile("ex1.vert");
            const char* fragmentShaderSource = Utils_ReadTextFile("ex1.frag");

            if (fragmentShaderSource && vertexShaderSource)
            {
                
                Shader vertex = CreateShader(GL_VERTEX_SHADER, vertexShaderSource);
                Utils_CheckShaderState(vertex, window);
                Shader fragment = CreateShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
                Utils_CheckShaderState(fragment, window);
                Program program = CreateShaderProgram(vertex, fragment);
                Utils_CheckProgramState(program, window);
                glUseProgram(program);

                SecondSolution(window);

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

// Create two shader programs where the second program uses a different fragment shader that outputs the color yellow; 
// draw both triangles again where one outputs the color yellow
void ThirdSolution()
{
    if (!glfwInit())
    {
        exit(1);
    }

    GLFWwindow* window = Utils_CreateWindow("Solution - 3");
    if (window)
    {
        glfwSetKeyCallback(window, KeyCallback);
        glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);  

        if (gladLoadGLLoader(glfwGetProcAddress))
        {
            const char* vertexShdrSrc = Utils_ReadTextFile("ex1.vert");
            const char* fragmentShdrSrcA = Utils_ReadTextFile("ex1.frag");
            const char* fragmentShdrSrcB = Utils_ReadTextFile("ex3.frag");

            int shadersLoaded = vertexShdrSrc && fragmentShdrSrcA && fragmentShdrSrcB;

            if (shadersLoaded)
            {
                Shader vert = CreateShader(GL_VERTEX_SHADER, vertexShdrSrc);
                Shader fragOrange = CreateShader(GL_FRAGMENT_SHADER, fragmentShdrSrcA);
                Shader fragYellow = CreateShader(GL_FRAGMENT_SHADER, fragmentShdrSrcB);

                Utils_CheckShaderState(vert, window);
                Utils_CheckShaderState(fragOrange, window);
                Utils_CheckShaderState(fragYellow, window);

                Program programOrange = CreateShaderProgram(vert, fragOrange);
                Program programYellow = CreateShaderProgram(vert, fragYellow);

                Utils_CheckProgramState(programOrange, window);
                Utils_CheckProgramState(programYellow, window);

                float verticesA[] =
                {
                        -1.0f, -0.5f, 0.0f, 
                         0.0f, -0.5f, 0.0f,    
                        -0.5,   0.5f, 0.0f
                };
                VBO vboA = CreateVBO(verticesA, ArraySize(verticesA));

                VAO vaoA = CreateVAO();
                glBindVertexArray(vaoA);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
                glEnableVertexAttribArray(0);

                float verticesB[] =
                {
                        0.0f, -0.5f, 0.0f,
                        1.0f, -0.5f, 0.0f,  
                        0.5f,  0.5f, 0.0f
                };

                VBO vboB = CreateVBO(verticesB, ArraySize(verticesB));

                VAO vaoB = CreateVAO();
                glBindVertexArray(vaoB);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
                glEnableVertexAttribArray(0);

                while (!glfwWindowShouldClose(window))
                {
                    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                    glClear(GL_COLOR_BUFFER_BIT);

                    glBindVertexArray(vaoA);
                    glUseProgram(programOrange);
                    glDrawArrays(GL_TRIANGLES, 0, 3);

                    glBindVertexArray(vaoB);
                    glUseProgram(programYellow);
                    glDrawArrays(GL_TRIANGLES, 0, 3);

                    glfwPollEvents();
                    glfwSwapBuffers(window);
                }
            }
        }
        glfwDestroyWindow(window);
    }
    glfwTerminate();
}

int main()
{
    // FirstAndSecondSolutionSetup();
    ThirdSolution();
}