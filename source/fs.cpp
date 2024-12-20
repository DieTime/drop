#include "fs.hpp"
#include "print.hpp"

#include <filesystem>

namespace {

std::string remove_trailing_slash(const std::string &path)
{
    if (path.ends_with('/')) {
        return std::filesystem::path(path).parent_path();
    }

    return path;
}

} /* namespace */

namespace fs {

entry::entry(const std::string &path)
    : m_path(remove_trailing_slash(path))
    , m_name(std::filesystem::path(m_path).filename())
    , m_absolute_path(std::filesystem::absolute(m_path))
{}

bool entry::copy(const entry &dest) const
{
    using std::filesystem::copy_options;

    std::error_code error_code;
    std::filesystem::copy(m_path,
                          dest.path(),
                          copy_options::recursive | copy_options::copy_symlinks,
                          error_code);

    if (error_code) {
        print::verbose("Couldn't copy " + m_path + " to " + dest.path(), error_code.message());
        return false;
    }

    return true;
}

bool entry::remove() const
{
    std::error_code error_code;
    bool is_success = std::filesystem::remove_all(m_path, error_code);

    if (error_code) {
        print::verbose("Couldn't remove " + m_path, error_code.message());
        return false;
    }

    if (!is_success) {
        print::verbose("Couldn't remove " + m_path, "The reason is unclear");
        return false;
    }

    return true;
}

bool entry::exists() const
{
    std::error_code error_code;
    bool is_symlink = std::filesystem::is_symlink(m_path);

    if (!error_code && is_symlink) {
        return true;
    }

    bool is_exists = std::filesystem::exists(m_path, error_code);

    if (error_code) {
        print::verbose("Couldn't check the existence of the " + m_path, error_code.message());
        return false;
    }

    return is_exists;
}

bool entry::create_as_directory() const
{
    std::error_code error_code;
    bool is_success = std::filesystem::create_directories(m_path, error_code);

    if (error_code) {
        print::verbose("Couldn't create directory " + m_path, error_code.message());
        return false;
    }

    if (!is_success) {
        print::verbose("Couldn't create directory " + m_path, "The reason is unclear");
        return false;
    }

    return true;
}

const std::string &entry::path() const
{
    return m_path;
}

const std::string &entry::absolute_path() const
{
    return m_absolute_path;
}

const std::string &entry::name() const
{
    return m_name;
}

entry entry::join(const std::string &path)
{
    return {m_path + "/" + path};
}

} /* namespace fs */
