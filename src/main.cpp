#include "ravicotis.hpp"
#include "common.hpp"
#include "logger.hpp"
#include "event.hpp"
#include <stdexcept>
#include <functional>
#include <csignal>

///Call this event to close program
rav::Event sigtermEvent;
///Handler for signal
void sigtermHandler(int sig)
{
    sigtermEvent.signal();
}

int main()
{
    rav::Logger::get().info(std::string(NAME).append(" ").append(VERSION));

    rav::Ravicotis app;
    std::signal(SIGINT, sigtermHandler);
    sigtermEvent.connect(std::bind(&rav::Ravicotis::close, &app));

    try
    {
        app.prepare();
        app.run();
    }
    catch (const std::runtime_error& e)
    {
        rav::Logger::get().error(e.what());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
