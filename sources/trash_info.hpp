#ifndef DROP_TRASH_INFO_HPP
#define DROP_TRASH_INFO_HPP

#include "fs.hpp"

namespace trash_info {

class writer
{
public:
    writer(fs::entry entry);

    bool write() const;

private:
    fs::entry m_entry;
};

} /* namespace trash_info */

#endif /* DROP_TRASH_INFO_HPP */
