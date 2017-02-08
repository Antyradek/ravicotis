#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<utility>
#include<memory>
#define GREEN_COLOR "\033[32m"
#define DEFAULT_COLOR "\033[39m"

namespace rav
{
/// \brief Logs info and errors in colours to STDOUT or given file.
class Logger
{
private:
    std::ostream* textOut;
    std::ostream* errOut;
    bool fileOutput;

    /// \brief If not file output, concatenate three strings, if file output, give only the middle.
    /// \param pre First part.
    /// \param text Second part.
    /// \param post Third part.
    /// \return Concatenated string, or only the middle one.
    std::string setColor(const std::string& pre, const std::string& text, const std::string& post) const;
    /// \brief Log in colour to STDOUT and STDERR.
    Logger();

    /// \brief Log to files.
    /// \param textOutFilename  File to log info to.
    /// \param errOutFilename  File to log errors to.
    Logger(const std::string& textOutFilename, const std::string& errOutFilename);

    virtual ~Logger();

public:
    /// \brief Log standard text.
    /// \param text  Text to write.
    void info(const std::string& text) const;

    /// \brief Log success in green colour.
    /// \param text Text to log
    void success(const std::string& text) const;

    /// \brief Get instance of logger.
    /// \return Instance of global logger.
    static Logger& get();

    /// \brief Copy constructor disabled in sigleton.
    Logger(const Logger& logger) = delete;

    /// \brief Operator disabled in singleton.
    void operator=(const Logger& logger)  = delete;
};
}
