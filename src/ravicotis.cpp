#include "ravicotis.hpp"
#include "logger.hpp"

#ifdef DEBUG
///Function for validation layer reporting

static VKAPI_ATTR VkBool32 VKAPI_CALL validationLayerCallback(
    VkDebugReportFlagsEXT flags,
    VkDebugReportObjectTypeEXT objType,
    uint64_t obj,
    size_t location,
    int32_t code,
    const char* layerPrefix,
    const char* msg,
    void* userData)
{
    rav::Logger::get().warning(msg);

    return VK_FALSE;
}
#endif // DEBUG

using namespace rav;
void Ravicotis::run()
{
    mainLoop();
}

void Ravicotis::prepare()
{
    initValidationLayers();
    initWindow();
    initVulkan();
    pickGPU();
    createDevice();
}

void Ravicotis::initValidationLayers()
{
#ifdef DEBUG
    //take alailable layers
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    VkLayerProperties availableLayers[layerCount];
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers);
    //check if the specified layer exists
    bool layerExists = false;
    for (uint32_t i = 0; i < layerCount; i++)
    {
        if(std::string(VALIDATION_LAYER_NAME).compare(availableLayers[i].layerName) == 0)
        {
            layerExists = true;
            break;
        }
    }
    if(!layerExists)
    {
        throw std::runtime_error(std::string(VALIDATION_LAYER_NAME).append(" not found!"));
    }
    else
    {
        Logger::get().success("All necessary validation layers exist.");
    }
#endif // DEBUG
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
    createInfo.enabledLayerCount = 0;
#ifdef DEBUG
    //Add validation layers to creation info
    createInfo.enabledLayerCount = VALIDATION_LAYER_COUNT;
    const char* layerNames[1];
    layerNames[0] = VALIDATION_LAYER_NAME;
    createInfo.ppEnabledLayerNames = layerNames;
#endif // DEBUG
    //Set GLFW extensions
    unsigned int glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
#ifdef DEBUG
    //Add reporting extension to creation info (add one extension to glfw extensions list)
    unsigned int extensionCount = glfwExtensionCount + 1;
    const char* extensions[extensionCount];
    for (unsigned int i = 0; i < glfwExtensionCount; i++)
    {
        extensions[i] = glfwExtensions[i];
    }
    extensions[extensionCount - 1] = VK_EXT_DEBUG_REPORT_EXTENSION_NAME;
    createInfo.enabledExtensionCount = extensionCount;
    createInfo.ppEnabledExtensionNames = extensions;
#endif // DEBUG
    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
    if(result != VK_SUCCESS) throw std::runtime_error(std::string("Failed to create Vulkan instance: ") + std::to_string(result));

#ifdef DEBUG
    //Set callback for reporting
    VkDebugReportCallbackCreateInfoEXT callbackCreateInfo = {};
    callbackCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    callbackCreateInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
    callbackCreateInfo.pfnCallback = validationLayerCallback;
    auto func = (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
    if(func == nullptr)
    {
        Logger::get().error("Validation layer callback setting function address couldn't be found.");
    }
    if(func(instance, &callbackCreateInfo, nullptr, &reportCallback) != VK_SUCCESS)
    {
        Logger::get().error("Could not set callback function.");
    }
    else
    {
        Logger::get().success("Successfully set callback function.");
    }
#endif // DEBUG
}

void Ravicotis::pickGPU()
{
    //Get GPU count
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    if (deviceCount == 0)
    {
        throw std::runtime_error("No GPUs with Vulkan support!");
    }
    VkPhysicalDevice devices[deviceCount];
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices);
#ifdef DEBUG
    std::string outText = "Found devices: \n";
    for(uint32_t i = 0; i < deviceCount; ++i)
    {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(devices[i], &deviceProperties);
        outText.append("\t").append(deviceProperties.deviceName).append("\n");
    }
    Logger::get().debug(outText);
#endif // DEBUG

    //Select appriopriate GPU
    for(uint32_t i = 0; i < deviceCount; ++i)
    {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(devices[i], &deviceProperties);
        //If has no queue families wth necessary values, seek next
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(devices[i], &queueFamilyCount, nullptr);
        VkQueueFamilyProperties queueFamilies[queueFamilyCount];
        vkGetPhysicalDeviceQueueFamilyProperties(devices[i], &queueFamilyCount, queueFamilies);
        bool hasFamily = false;
        for(uint32_t j = 0; j < queueFamilyCount; ++j)
        {
            if(queueFamilies[j].queueCount <= 0)
            {
                continue;
            }
            if(queueFamilies[j].queueFlags & VK_QUEUE_GRAPHICS_BIT == 0)
            {
                continue;
            }
            hasFamily = true;
        }
        if(!hasFamily)
        {
            continue;
        }
        physicalDevice = devices[i];
        Logger::get().success(std::string("Selected device: ").append(deviceProperties.deviceName));
        return;
    }

    throw std::runtime_error("No GPU with necessary features found!");
}

void Ravicotis::createDevice()
{
    //Get queue families for GPU and select first appriopriate family
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
    VkQueueFamilyProperties queueFamilies[queueFamilyCount];
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies);
    uint32_t wantedQueueFamilyIndex = 0;
    for(uint32_t j = 0; j < queueFamilyCount; ++j)
    {
        if(queueFamilies[j].queueCount <= 0)
        {
            continue;
        }
        if(queueFamilies[j].queueFlags & VK_QUEUE_GRAPHICS_BIT == 0)
        {
            continue;
        }
        wantedQueueFamilyIndex = j;
        break;
    }
    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = wantedQueueFamilyIndex;
    queueCreateInfo.queueCount = 1;
    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures = {};

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = 0;
    createInfo.enabledLayerCount = 0;
#ifdef DEBUG
    createInfo.enabledLayerCount = VALIDATION_LAYER_COUNT;
    const char* layerNames[1];
    layerNames[0] = VALIDATION_LAYER_NAME;
    createInfo.ppEnabledLayerNames = layerNames;
#endif // DEBUG

    VkResult result =  vkCreateDevice(physicalDevice, &createInfo, nullptr, &device);
    if(result != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create logical device");
    }

    vkGetDeviceQueue(device, wantedQueueFamilyIndex, 0, &graphicsQueue);

    Logger::get().success("Created logical device");
}

void Ravicotis::clean()
{
    Logger::get().info("Closing application");
#ifdef DEBUG
    //Unset reporting callback
    auto func = (PFN_vkDestroyDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
    if(func == nullptr)
    {
        Logger::get().error("Validation layer callback unsetting function address couldn't be found.");
    }
    func(instance, reportCallback, nullptr);
#endif // DEBUG
    vkDestroyDevice(device, nullptr);
    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(window);
}

void Ravicotis::mainLoop()
{
    isRunning = true;
    isClosedExternally = false;
    while (!shouldClose())
    {
        calcEvents();
    }

    clean();
}

bool Ravicotis::shouldClose()
{
    return !isRunning;
}

void Ravicotis::calcEvents()
{
    glfwPollEvents();

    //mutex in case close() is called
    closeMutex.lock();
    if(isRunning)
    {
        if(glfwWindowShouldClose(window) || isClosedExternally)
        {
            isRunning = false;
        }
    }
    closeMutex.unlock();
}

void Ravicotis::close()
{
    closeMutex.lock();
    isClosedExternally = true;
    closeMutex.unlock();
}

