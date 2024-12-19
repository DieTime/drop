#include "trash_info.hpp"

#include <chrono>
#include <fcntl.h>
#include <iomanip>
#include <sstream>
#include <unistd.h>

namespace {

bool write_n_bytes(int fd, const char *data, size_t data_size)
{
    size_t cursor = 0;
    size_t chunk_size = BUFSIZ;

    while (cursor != data_size) {
        size_t bytes_to_write = std::min(chunk_size, data_size - cursor);
        ssize_t bytes_written = write(fd, data + cursor, bytes_to_write);

        if (bytes_written == -1) {
            return false;
        }

        cursor += bytes_written;
    }

    return true;
}

} /* namespace */

namespace trash_info {

writer::writer(std::string path_to_be_removed)
    : m_path_to_be_removed(std::move(path_to_be_removed))
{}

bool writer::write_to(const std::string &dest) const
{
    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm *time = std::localtime(&now);

    if (!time) {
        return false;
    }

    /**
     * Create and open destination file usign open() with O_EXCL:
     * https://specifications.freedesktop.org/trash-spec/1.0/#contents-of-a-trash-directory
     */
    int fd = open(dest.c_str(), O_CREAT | O_WRONLY | O_EXCL);

    if (fd == -1) {
        return false;
    }

    std::stringstream content_stream;
    content_stream << "[Trash Info]" << std::endl;
    content_stream << "Path=" << m_path_to_be_removed << std::endl;
    content_stream << "DeletionDate=" << std::put_time(time, "%Y-%m-%dT%H:%M:%S") << std::endl;

    std::string content = content_stream.str();
    bool ok = write_n_bytes(fd, content.data(), content.size());

    close(fd);
    return ok;
}

} /* namespace trash_info */
