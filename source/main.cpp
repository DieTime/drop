/**
 * SPDX-FileCopyrightText: 2024 Denis Glazkov <glazzk.off@mail.ru>
 * SPDX-License-Identifier: MIT
 */

#include "fs.hpp"
#include "print.hpp"
#include "trash_info.hpp"
#include "uuid.hpp"
#include "version.hpp"

#include <limits>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace {

std::string about()
{
    return "Drop " DROP_VERSION "\n"
           "  - Commit: " DROP_COMMIT_HASH "\n"
           "  - Tag: " DROP_TAG "\n"
           "  - Remote: " DROP_REMOTE;
}

std::string usage()
{
    return "Drop anything in the trash.\n"
           "\n"
           "Usage:\n"
           "   drop [OPTIONS] PATH [PATH...]\n"
           "\n"
           "Options:\n"
           "   -h,--help       Print help message\n"
           "   -a,--about      Print about message\n"
           "   -v,--verbose    Print verbose messages";
}

bool drop_path(const std::string &path)
{
    fs::entry entry(path);

    if (entry.path() == "/") {
        print::oops("Nice try!", "Did you really want to remove all system files?");
        return false;
    }

    if (!entry.exists()) {
        print::error("The " + entry.path() + " doesn't exists");
        return false;
    }

    const char *user_home_directory = std::getenv("HOME");

    if (!user_home_directory) {
        print::oops("Couldn't drop " + entry.path(),
                    "The $HOME environment variable is not defined");
        return false;
    }

    fs::entry trash = fs::entry(user_home_directory).join(".local").join("share").join("Trash");
    fs::entry trash_files_directory = trash.join("files");
    fs::entry trash_info_directory = trash.join("info");

    if (entry.absolute_path() == trash.path()) {
        print::oops("Nice try!", "Did you really want to remove the trash to trash?");
        return false;
    }

    if (entry.absolute_path().starts_with(trash.path() + "/")) {
        print::oops("Nice try!", "Why do you want to remove files that are already in the trash?");
        return false;
    }

    if (!trash.exists() && !trash.create_as_directory()) {
        print::oops("Couldn't drop " + entry.path(), "Couldn't create directory " + trash.path());
        return false;
    }

    if (!trash_files_directory.exists() && !trash_files_directory.create_as_directory()) {
        print::oops("Couldn't drop " + entry.path(),
                    "Couldn't create directory " + trash_files_directory.path());
        return false;
    }

    if (!trash_info_directory.exists() && !trash_info_directory.create_as_directory()) {
        print::oops("Couldn't drop " + entry.path(),
                    "Couldn't create directory " + trash_info_directory.path());
        return false;
    }

    fs::entry trash_entry = trash_files_directory.join(entry.name());
    fs::entry trash_info_entry = trash_info_directory.join(entry.name() + ".trashinfo");

    size_t attempts = 0;

    do {
        if (attempts >= 10) {
            print::oops("Couldn't drop " + entry.path(),
                        "Couldn't resolve name collisions in the trash directory");
            return false;
        }

        std::string unique_entry_name = entry.name() + " (drop-uuid=" + uuid::v4() + ")";
        trash_entry = trash_files_directory.join(unique_entry_name);
        trash_info_entry = trash_info_directory.join(unique_entry_name + ".trashinfo");

        print::verbose("Attempting to select a file name for the trash", unique_entry_name);
        attempts += 1;
    } while (trash_entry.exists() || trash_info_entry.exists());

    trash_info::writer trash_info_writer(entry.absolute_path());

    if (!trash_info_writer.write_to(trash_info_entry.path())) {
        print::oops("Couldn't drop " + entry.path(),
                    "Couldn't create trash info " + trash_info_entry.path());
        return false;
    }

    print::verbose("Trash info successfully created", trash_info_entry.path());

    if (!entry.copy(trash_entry)) {
        print::oops("Couldn't drop " + entry.path(),
                    "Couldn't copy " + entry.path() + " to " + trash_entry.path());

        print::verbose("Removing created trash info", trash_info_entry.path());
        trash_info_entry.remove();

        return false;
    }

    print::verbose("File successfully copied to the trash", trash_entry.path());

    if (!entry.remove()) {
        print::oops("Couldn't drop " + entry.path(), "Couldn't remove " + entry.path());

        print::verbose("Removing created trash info", trash_info_entry.path());
        trash_info_entry.remove();

        print::verbose("Removing copied trash entry", trash_entry.path());
        trash_entry.remove();

        return false;
    }

    print::verbose("File successfully removed", entry.path());
    return true;
}

} /* namespace */

int main(int argc, char **argv)
{
    std::span<char *> args(argv + 1, argc - 1);
    std::vector<std::string> paths;

    while (!args.empty()) {
        if (args[0] == std::string_view("-h") || args[0] == std::string_view("--help")) {
            print::message(usage());
            return 0;
        }

        if (args[0] == std::string_view("-a") || args[0] == std::string_view("--about")) {
            print::message(about());
            return 0;
        }

        if (args[0] == std::string_view("-v") || args[0] == std::string_view("--verbose")) {
            print::set_verbose_mode(print::verbose_mode::enabled);
        } else if (fs::entry(args[0]).exists()) {
            paths.emplace_back(args[0]);
        } else {
            print::verbose("Skip dropping a non-existent file", args[0]);
        }

        args = args.subspan(1);
    }

    bool success = true;

    for (const auto &path: paths) {
        if (!drop_path(path)) {
            success = false;
        }
    }

    return success ? 0 : 1;
}
