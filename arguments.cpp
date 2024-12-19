#include "arguments.hpp"

#include <cassert>

namespace arguments {

container::container(int argc, char **argv)
    : m_arguments(argv, argc)
{}

size_t container::count() const
{
    return m_arguments.size();
}

std::string_view container::at(size_t index) const
{
    assert(index < m_arguments.size());
    return m_arguments[index];
}

} /* namespace arguments */
