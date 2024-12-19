#include "fs.hpp"
#include "print.hpp"
#include "trash_info.hpp"
#include "version.hpp"

#include <limits>
#include <sstream>
#include <string>

namespace {

std::string version_message()
{
    std::stringstream version;
    version << "Drop " << DROP_VERSION << std::endl
            << "  - Commit: " << DROP_COMMIT_HASH << std::endl
            << "  - Tag: " << DROP_TAG << std::endl
            << "  - Remote: " << DROP_REMOTE;

    return version.str();
}

std::string usage_message()
{
    std::stringstream usage;
    usage << "Usage:" << std::endl
          << "   drop PATH            Remove PATH to the trash" << std::endl
          << "   drop -h|--help       Print help message" << std::endl
          << "   drop -v|--version    Print version";

    return usage.str();
}

} /* namespace */

int main(int argc, char **argv)
{
    if (argc != 2) {
        print::error("Invalid arguments count");
        return 1;
    }

    std::string argument = argv[1];

    if (argument == "-v" || argument == "--version") {
        print::message(version_message());
        return 0;
    }

    if (argument == "-h" || argument == "--help") {
        print::message(usage_message());
        return 0;
    }

    fs::entry entry(argument);

    if (!entry.exists()) {
        print::error(entry.path() + " doesn't exists");
        return 1;
    }

    const char *user_home_directory = std::getenv("HOME");

    if (!user_home_directory) {
        print::oops("Couldn't drop " + entry.path(),
                    "The $HOME environment variable is not defined");
        return 1;
    }

    fs::entry trash = fs::entry(user_home_directory).join(".local").join("share").join("Trash");
    fs::entry trash_files_directory = trash.join("files");
    fs::entry trash_info_directory = trash.join("info");

    if (!trash.exists() && !trash.create_as_directory()) {
        print::oops("Couldn't drop " + entry.path(), "Couldn't create directory " + trash.path());
        return 1;
    }

    if (!trash_files_directory.exists() && !trash_files_directory.create_as_directory()) {
        print::oops("Couldn't drop " + entry.path(),
                    "Couldn't create directory " + trash_files_directory.path());
        return 1;
    }

    if (!trash_info_directory.exists() && !trash_info_directory.create_as_directory()) {
        print::oops("Couldn't drop " + entry.path(),
                    "Couldn't create directory " + trash_info_directory.path());
        return 1;
    }

    fs::entry trash_entry = trash_files_directory.join(entry.name());
    fs::entry trash_info_entry = trash_info_directory.join(entry.name() + ".trashinfo");

    if (trash_entry.exists()) {
        for (int i = 1; i < std::numeric_limits<int>::max(); i++) {
            std::string entry_name = entry.name() + " (" + std::to_string(i) + ")";

            trash_entry = trash.join("files").join(entry_name);
            trash_info_entry = trash.join("info").join(entry_name + ".trashinfo");

            if (!trash_entry.exists()) {
                break;
            }
        }
    }

    if (trash_entry.exists()) {
        print::oops("Couldn't drop " + entry.path(),
                    "Couldn't create a unique name for the trash directory");
        return 1;
    }

    trash_info::writer trash_info_writer(entry.absolute_path());

    if (!trash_info_writer.write_to(trash_info_entry.path())) {
        print::oops("Couldn't drop " + entry.path(),
                    "Couldn't create trash info " + trash_info_entry.path());
        return 1;
    }

    if (!entry.copy(trash_entry)) {
        print::oops("Couldn't drop " + entry.path(),
                    "Couldn't copy " + entry.path() + " to " + trash_entry.path());
        trash_info_entry.remove();
        return 1;
    }

    if (!entry.remove()) {
        print::oops("Couldn't drop " + entry.path(), "Couldn't remove " + entry.path());
        trash_info_entry.remove();
        trash_entry.remove();
        return 1;
    }

    return 0;
}
