#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <string_view>
#include <string>

#include "version.hpp"

namespace fs = std::filesystem;
namespace ch = std::chrono;

void fs_entry_copy(const fs::path &from, const fs::path &to)
{
    if (fs::is_symlink(from)) {
        fs::copy_symlink(from, to);
    } else {
        fs::copy(from, to);
    }
}

bool fs_entry_exists(const fs::path &path) {
    return fs::is_symlink(path) || fs::exists(path);
}

void fs_entry_cleanup(const fs::path &path)
{
    if (fs_entry_exists(path)) {
        try {
            fs::remove_all(path);
        }
        catch (const std::exception &err) {
            std::cout << "😥 couldn't clean up " << path << std::endl
                      << "   ↳ " << err.what() << std::endl;
        }
    }
}

void create_trash_info(const fs::path &entry_path, const fs::path &info_path)
{
    std::time_t now = ch::system_clock::to_time_t(ch::system_clock::now());
    std::tm *tm = std::localtime(&now);

    if (!tm) {
        throw std::runtime_error("could not get the current time");
    }

    std::ofstream out(info_path, std::ios::trunc);

    if (!out) {
        throw std::runtime_error("could not open " + info_path.string() + " for writting");
    }

    out << "[Trash Info]" << std::endl;
    out << "Path=" << fs::absolute(entry_path) << std::endl;
    out << "DeletionDate=" << std::put_time(tm, "%Y-%m-%dT%H:%M:%S") << std::endl;
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        std::cout << "😥 path is not specified" << std::endl;
        return 1;
    }

    if (std::string_view(argv[1]) == "--version") {
        std::cout << "drop " << DROP_VERSION << std::endl
                  << " - commit: " << DROP_COMMIT_HASH << std::endl
                  << " - tag: " << DROP_TAG << std::endl
                  << " - remote: " << DROP_REMOTE << std::endl;
        return 0;
    }

    if (std::string_view(argv[1]) == "--help") {
        std::cout << "Remove anything to the trash." << std::endl << std::endl
                  << "Usage:" << std::endl
                  << "   drop <PATH>" << std::endl;
        return 0;
    }

    fs::path entry_path = argv[1];
    std::string entry_name = entry_path.filename().string();

    if (!std::getenv("HOME")) {
        std::cout << "😥 couldn't drop " << entry_path << std::endl
                  << "   ↳ user home directory not defined" << std::endl;
        return 1;
    }

    fs::path trash_directory = fs::path(std::getenv("HOME")) / ".local" / "share" / "Trash";

    try {
        fs::create_directories(trash_directory);
    }
    catch (const std::exception &err) {
        std::cout << "😥 couldn't drop " << entry_path << std::endl
                << "   ↳ " << err.what() << std::endl;
    }

    fs::path trash_entry_path = trash_directory / "files" / entry_name;
    fs::path trash_entry_info_path = trash_directory / "info" / (entry_name + ".trashinfo");

    if (fs_entry_exists(trash_entry_path)) {
        for (int i = 1; i < std::numeric_limits<int>::max(); i++) {
            std::string entry_name_with_num = entry_name + " (" + std::to_string(i) + ")";

            trash_entry_path = trash_directory / "files" / entry_name_with_num;
            trash_entry_info_path = trash_directory / "info" / (entry_name_with_num + ".trashinfo");

            if (!fs_entry_exists(trash_entry_path)) {
                break;
            }
        }
    }

    if (fs_entry_exists(trash_entry_path)) {
        std::cout << "😥 couldn't drop " << entry_path << std::endl
                  << "   ↳ path " << trash_entry_path << " already exists in the trash"
                  << std::endl;
        return 1;
    }

    try {
        fs_entry_copy(entry_path, trash_entry_path);
        create_trash_info(entry_path, trash_entry_info_path);
        fs::remove_all(entry_path);
    }
    catch (const std::exception &err) {
        std::cout << "😥 couldn't drop " << entry_path << std::endl
                  << "   ↳ " << err.what() << std::endl;

        fs_entry_cleanup(trash_entry_path);
        fs_entry_cleanup(trash_entry_info_path);

        return 1;
    }

    return 0;
}
