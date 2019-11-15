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
    
    int w, h, channelCount;
    unsigned char imageData = stbi_load("graphite.jpg", &w, &h, &channelCount, 0);
    assert(imageData);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
    glGenerateMipmap(texture);

    float vertices[] = 
    {
        -0.5f, 0.0f, 0.0f, /* */ 0.0f, 0.0f, 
         0.5f, 0.0f, 0.0f, /* */ 1.0f, 0.0f, 
         0.0f, 1.0f, 0.0f, /* */ 0.5f, 0.0f, 
    };

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

}