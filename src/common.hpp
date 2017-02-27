#pragma once

#define NAME "Ravicotis"
#define VERSION_MAJOR 0
#define VERSION_MINOR 1
#define VERSION_PATCH 1

#define VERSION std::to_string(VERSION_MAJOR)).append(".").append(std::to_string(VERSION_MINOR)).append(".").append(std::to_string(VERSION_PATCH)

#define WINDOW_WIDTH 2000
#define WINDOW_HEIGHT 2000

#ifdef DEBUG
    #define VALIDATION_LAYER_COUNT 1
    #define VALIDATION_LAYER_NAME "VK_LAYER_LUNARG_standard_validation"
#endif // DEBUG
