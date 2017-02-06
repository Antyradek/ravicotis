#include "ravicotis.hpp"

using namespace rav;
void Ravicotis::run()
{
    initWindow();
    initVulkan();
    mainLoop();
}

void Ravicotis::initWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    std::string windowTitle;
    windowTitle.append(NAME).append(" ").append(VERSION);
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, windowTitle.c_str(), nullptr, nullptr);
}

void Ravicotis::initVulkan()
{

}

void Ravicotis::mainLoop()
{
    isRunning = true;
    while (!shouldClose())
    {
        calcEvents();
    }

    glfwDestroyWindow(window);
}

Ravicotis::Ravicotis()
{

}

bool Ravicotis::shouldClose()
{
    return !isRunning;
}

void Ravicotis::calcEvents()
{
    glfwPollEvents();

    if(isRunning)
    {
        if(glfwWindowShouldClose(window))
        {
            close();
        }
    }
}

void Ravicotis::close()
{
    glfwSetWindowShouldClose(window, GLFW_TRUE);
    isRunning = false;
    std::cout << "Closing" << std::endl;
}

