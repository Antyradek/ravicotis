#include "ravicotis.hpp"
#include "logger.hpp"

using namespace rav;
void Ravicotis::run()
{
    mainLoop();
}

void Ravicotis::prepare()
{
    initWindow();
    initVulkan();
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
    //Create vulkan instance
    //App info
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = NAME;
    appInfo.applicationVersion = VK_MAKE_VERSION(VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
    appInfo.pEngineName = "Rav";
    appInfo.engineVersion = VK_MAKE_VERSION(VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
    appInfo.apiVersion = VK_API_VERSION_1_0;
    //Instance creation info
    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    //Set GLFW extensions
    unsigned int glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    createInfo.enabledLayerCount = 0;
    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
    if(result != VK_SUCCESS) throw std::runtime_error(std::string("Failed to create Vulkan instance: ") + std::to_string(result));

}

void Ravicotis::clean()
{
    vkDestroyInstance(instance, nullptr);
}

void Ravicotis::mainLoop()
{
    isRunning = true;
    while (!shouldClose())
    {
        calcEvents();
    }
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
    Logger::get().info("Closing application");
    glfwDestroyWindow(window);
    clean();
}

