#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "utils/utils.h"

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

int main()
{
    assert(glfwInit());
    GLFWwindow* window = Utils_CreateWindow("Textures exercises");
    assert(window);
    assert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));

    glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
    glfwSetKeyCallback(window, KeyCallback);

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