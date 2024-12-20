#ifndef DROP_FS_HPP
#define DROP_FS_HPP

#include <string>
#include <string_view>

namespace fs {

class entry
{
public:
    entry() = default;
    entry(std::string path);

    bool copy(const entry &dest) const;
    bool remove() const;
    bool exists() const;
    bool create_as_directory() const;

    const std::string &path() const;
    const std::string &absolute_path() const;
    const std::string &name() const;

    entry join(const std::string &path);

private:
    std::string m_path;
    std::string m_name;
    std::string m_absolute_path;
};

} /* namespace fs */

#endif /* DROP_FS_HPP */
