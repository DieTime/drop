#include "print.hpp"

#include <iostream>
#include <string_view>
#include <unistd.h>

namespace {

print::verbose_mode current_verbose_mode = print::verbose_mode::disabled;

std::string_view red()
{
    return isatty(fileno(stdout)) ? "\033[1;31m" : "";
}

std::string_view yellow()
{
    return isatty(fileno(stdout)) ? "\033[1;33m" : "";
}

std::string_view gray()
{
    return isatty(fileno(stdout)) ? "\033[2m" : "";
}

std::string_view white()
{
    return isatty(fileno(stdout)) ? "\033[1;37m" : "";
}

std::string_view reset()
{
    return isatty(fileno(stdout)) ? "\033[0m" : "";
}

} /* namespace */

namespace print {

void set_verbose_mode(verbose_mode mode)
{
    current_verbose_mode = mode;
}

void error(const std::string &message)
{
    std::cout << "📢 " << red() << "Error! " << reset() << message << std::endl;
}

void oops(const std::string &message, const std::string &description)
{
    std::cout << "😢 " << yellow() << "Oops! " << reset() << message << std::endl
              << gray() << "   ↳ " << description << reset() << std::endl;
}

void message(const std::string &message)
{
    std::cout << message << std::endl;
}

void verbose(const std::string &message, const std::string &description)
{
    if (current_verbose_mode == verbose_mode::enabled) {
        std::cout << "👀 " << white() << "Verbose! " << reset() << message << std::endl
                  << gray() << "   ↳ " << description << reset() << std::endl;
    }
}

} /* namespace print */
