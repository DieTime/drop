#include "fs.hpp"

#include <filesystem>

namespace fs {

entry::entry(std::string path)
    : m_path(std::move(path))
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

    return !error_code;
}

bool entry::remove() const
{
    std::error_code error_code;
    bool is_success = std::filesystem::remove_all(m_path, error_code);

    return !error_code && is_success;
}

bool entry::exists() const
{
    std::error_code error_code;
    bool is_exists = std::filesystem::exists(m_path, error_code);

    return is_symlink() || (!error_code && is_exists);
}

bool entry::is_symlink() const
{
    std::error_code error_code;
    bool is_symlink = std::filesystem::is_symlink(m_path, error_code);

    return !error_code && is_symlink;
}

bool entry::create_as_directory() const
{
    std::error_code error_code;
    bool is_success = std::filesystem::create_directories(m_path, error_code);

    return !error_code && is_success;
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
