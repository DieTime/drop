#include "print.hpp"

#include <iostream>
#include <string_view>
#include <unistd.h>

namespace {

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

std::string_view reset()
{
    return isatty(fileno(stdout)) ? "\033[0m" : "";
}

} /* namespace */

namespace print {

void error(const std::string &message)
{
    std::cout << "📢 " << red() << "Error! " << reset() << message << std::endl;
}

void oops(const std::string &message, const std::string &description)
{
    std::cout << "😢 " << yellow() << "Oops... " << reset() << message << std::endl
              << gray() << "   ↳ " << description << reset() << std::endl;
}

void message(const std::string &message)
{
    std::cout << message << std::endl;
}

} /* namespace print */
