#include "trash_info.hpp"

#include <chrono>
#include <fstream>
#include <iomanip>

namespace trash_info {

writer::writer(fs::entry entry)
    : m_entry(std::move(entry))
{}

bool writer::write() const
{
    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm *tm = std::localtime(&now);

    if (!tm) {
        return false;
    }

    std::ofstream output(m_entry.path(), std::ios::trunc);
    output << "[Trash Info]" << std::endl;
    output << "Path=" << m_entry.absolute_path() << std::endl;
    output << "DeletionDate=" << std::put_time(tm, "%Y-%m-%dT%H:%M:%S") << std::endl;

    if (!output) {
        return false;
    }

    return true;
}

} /* namespace trash_info */
