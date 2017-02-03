#pragma once
#include<iostream>
#include<vulkan/vulkan.hpp>


namespace rav
{

/// \brief Main application object.
class Ravicotis
{
public:
    /// \brief Start application.
    void run();

    /// \brief Initialize Vulkan.
    void initVulkan();

    /// \brief Main loop, where everything is made.
    void mainLoop();
};
}
