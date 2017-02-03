#include "ravicotis.hpp"
#include "common.hpp"
#include "logger.hpp"
#include <stdexcept>
#include <functional>

int main()
{
    rav::Logger logger;
    logger.info("RAVICOTIS " + VERSION);

    rav::Ravicotis app;
    try
    {
        app.run();
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
