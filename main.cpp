#include "arguments.hpp"
#include "fs.hpp"
#include "print.hpp"
#include "trash_info.hpp"
#include "version.hpp"

#include <limits>
#include <sstream>

namespace {

std::string version_message()
{
    std::stringstream version;
    version << "drop " << DROP_VERSION << std::endl
            << "  - commit: " << DROP_COMMIT_HASH << std::endl
            << "  - tag: " << DROP_TAG << std::endl
            << "  - remote: " << DROP_REMOTE;

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
    arguments::container args(argc, argv);

    if (args.count() != 2) {
        print::error("Invalid arguments count");
        return 1;
    }

    if (args.at(1) == "-v" || args.at(1) == "--version") {
        print::message(version_message());
        return 0;
    }

    if (args.at(1) == "-h" || args.at(1) == "--help") {
        print::message(usage_message());
        return 0;
    }

    fs::entry entry(std::string(args.at(1)));

    if (!entry.exists()) {
        print::error(entry.path() + " doesn't exists");
        return 1;
    }

    if (!std::getenv("HOME")) {
        print::oops("Couldn't drop " + entry.path(), "User home directory not defined");
        return 1;
    }

    fs::entry trash = fs::entry(std::getenv("HOME")).join(".local").join("share").join("Trash");

    if (!trash.exists()) {
        print::oops("Couldn't drop " + entry.path(), "Trash doesn't exists: " + trash.path());
        return 1;
    }

    fs::entry trash_entry = trash.join("files").join(entry.name());
    fs::entry trash_info_entry = trash.join("info").join(entry.name() + ".trashinfo");

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

    if (!entry.copy(trash_entry)) {
        print::oops("Couldn't drop " + entry.path(),
                    "Couldn't copy " + entry.path() + " to " + trash_entry.path());
        return 1;
    }

    trash_info::writter trash_info_writter(trash_info_entry);

    if (!trash_info_writter.write()) {
        print::oops("Couldn't drop " + entry.path(),
                    "Couldn't create trash info " + trash_info_entry.path());
        trash_entry.remove();
        return 1;
    }

    if (!entry.remove()) {
        print::oops("Couldn't drop " + entry.path(), "Couldn't remove " + entry.path());
        trash_entry.remove();
        trash_info_entry.remove();
        return 1;
    }

    return 0;
}
