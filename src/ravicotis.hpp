#pragma once
#include<iostream>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "common.hpp"


namespace rav
{

/// \brief Main application object.
class Ravicotis
{
public:
    /// \brief Prepare application to start.
    void prepare();

    /// \brief Start application.
    void run();

    /// \brief Close application.
    void close();
private:
    /// \brief Create GLFW window.
    void initWindow();

    /// \brief Initialize Vulkan.
    void initVulkan();

    /// \brief Main loop, where everything is made.
    void mainLoop();

    /// \brief Deallocate memory.
    void clean();

    /// \brief Program should close.
    /// \return If user asked program to close.
    bool shouldClose();

    /// \brief Think about events.
    void calcEvents();


    /// Main GLFW window
    GLFWwindow* window;

    /// Is running
    bool isRunning;

    /// Main VK instance
    VkInstance instance;
};
}
