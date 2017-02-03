#include "logger.hpp"

using namespace rav;
Logger::Logger()
{
    textOut = &std::cout;
    errOut = &std::cerr;
    fileOutput = false;
}

Logger::Logger(const std::string& textOutFilename, const std::string& errOutFilename)
{
    fileOutput = true;
    textOut = new std::ofstream(textOutFilename, std::ofstream::out);
    errOut = new std::ofstream(errOutFilename, std::ofstream::out);
}

Logger::~Logger()
{
    if(fileOutput)
    {
        delete(textOut);
        delete(errOut);
    }
}

void Logger::info(const std::string& text) const
{
    *textOut << text << std::endl;
}

void Logger::success(const std::string& text) const
{
    *textOut << setColor(GREEN_COLOR, text, DEFAULT_COLOR) << std::endl;
}

std::string Logger::setColor(const std::string& pre, const std::string& text, const std::string& post) const
{
    if(!fileOutput)
    {
        return pre + text + post;
    }
    else
    {
        return text;
    }
}
