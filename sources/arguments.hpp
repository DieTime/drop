#ifndef DROP_ARGUMENTS_HPP
#define DROP_ARGUMENTS_HPP

#include <span>
#include <string_view>

namespace arguments {

class container
{
public:
    container(int argc, char **argv);

    std::string_view at(size_t index) const;
    size_t count() const;

private:
    std::span<char *> m_arguments;
};

} /* namespace arguments */

#endif /* DROP_ARGUMENTS_HPP */
