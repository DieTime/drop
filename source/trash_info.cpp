/**
 * SPDX-FileCopyrightText: 2024 Denis Glazkov <glazzk.off@mail.ru>
 * SPDX-License-Identifier: MIT
 */

#include "trash_info.hpp"
#include "print.hpp"

#include <cerrno>
#include <chrono>
#include <cstring>
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
            print::verbose("Couldn't write " + std::to_string(bytes_to_write) + " bytes to "
                               + std::to_string(fd) + " file descriptor",
                           std::strerror(errno));
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
        print::verbose("Couldn't parse unix timestamp", std::strerror(errno));
        return false;
    }

    /**
     * Create and open destination file using open() with O_EXCL:
     * https://specifications.freedesktop.org/trash-spec/1.0/#contents-of-a-trash-directory
     */
    int fd = open(dest.c_str(), O_CREAT | O_WRONLY | O_EXCL, 0644);

    if (fd == -1) {
        print::verbose("Couldn't open " + dest + " for writing", std::strerror(errno));
        return false;
    }

    std::stringstream content_stream;
    content_stream << "[Trash Info]" << std::endl;
    content_stream << "Path=" << m_path_to_be_removed << std::endl;
    content_stream << "DeletionDate=" << std::put_time(time, "%Y-%m-%dT%H:%M:%S") << std::endl;

    std::string content = content_stream.str();

    bool ok = write_n_bytes(fd, content.data(), content.size());
    close(fd);

    if (!ok) {
        print::verbose("Couldn't write " + std::to_string(content.size()) + " bytes to " + dest,
                       std::strerror(errno));
        return false;
    }

    return true;
}

} /* namespace trash_info */
