#include "ravicotis.hpp"
#include "common.hpp"
#include "logger.hpp"
#include <stdexcept>
#include <functional>
#include <csignal>

void sigtermHandler(int sig)
{
    //app.close();
}

int main()
{
    rav::Logger logger;
    logger.info(std::string(NAME).append(" ").append(VERSION));

    rav::Ravicotis app;
    std::signal(SIGTERM, sigtermHandler);
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
