#include "ravicotis.hpp"

void rav::Ravicotis::run()
{
    initWindow();
    initVulkan();
    mainLoop();
}

void rav::Ravicotis::initWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    std::string windowTitle;
    windowTitle.append(NAME).append(" ").append(VERSION);
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, windowTitle.c_str(), nullptr, nullptr);
}

void rav::Ravicotis::initVulkan()
{

}

void rav::Ravicotis::mainLoop()
{
    while (!shouldClose())
    {
        calcEvents();
    }
    close();
}

rav::Ravicotis::Ravicotis()
{

}

bool rav::Ravicotis::shouldClose()
{
    return glfwWindowShouldClose(window);
}

void rav::Ravicotis::calcEvents()
{
    glfwPollEvents();
}

void rav::Ravicotis::close()
{
    glfwDestroyWindow(window);
}

