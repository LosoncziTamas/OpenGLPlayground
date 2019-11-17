#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "utils/utils.h"
#define STB_IMAGE_IMPLEMENTATION
#include "utils/stb_image.h"

#include <assert.h>

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

// ex-1: Make sure only the happy face looks in the other/reverse direction by changing the fragment shader.

int main()
{
    assert(glfwInit());
    GLFWwindow* window = Utils_CreateWindow("Textures exercises");
    assert(window);
    assert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));

    glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
    glfwSetKeyCallback(window, KeyCallback);

    const char* vertSrc = Utils_ReadTextFile("texture.vert");
    const char* fragSrc = Utils_ReadTextFile("texture.frag");
    assert(vertSrc);
    assert(fragSrc);

    GLuint vertShdr = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShdr, 1, &vertSrc, 0);
    glCompileShader(vertShdr);
    Utils_CheckShaderState(vertShdr, window);

    GLuint fragShdr = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShdr, 1, &fragSrc, 0);
    glCompileShader(fragShdr);
    Utils_CheckShaderState(fragShdr, window);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertShdr);
    glAttachShader(program, fragShdr);
    glLinkProgram(program);
    Utils_CheckProgramState(program, window);

    glUseProgram(program);
    
    int w, h, channelCount;
    unsigned char* imageData = stbi_load("graphite.jpg", &w, &h, &channelCount, 0);
    assert(imageData);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
    glGenerateMipmap(texture);

    float vertices[] = 
    {
        -0.5f, 0.0f, 0.0f, /* */ 0.0f, 0.0f, 
         0.5f, 0.0f, 0.0f, /* */ 1.0f, 0.0f, 
         0.0f, 1.0f, 0.0f, /* */ 0.5f, 1.0f, 
    };

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

}