#pragma once
#include<iostream>
#include<mutex>

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

    /// \brief Close application, can be called by other thread (such as signal SIGINT).
    void close();
private:
    /// \brief Init validation layers.
    void initValidationLayers();

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


    /// Main GLFW window.
    GLFWwindow* window;

    /// Is everything running?
    bool isRunning;

    /// Closed with close().
    bool isClosedExternally;

    /// Main VK instance.
    VkInstance instance;

    /// Mutex to allow asynchronous call to close app.
    std::mutex closeMutex;

    /// Are validation layers enabled?
    bool enableValidationLayers;
};
}
