/**
 * SPDX-FileCopyrightText: 2024 Denis Glazkov <glazzk.off@mail.ru>
 * SPDX-License-Identifier: MIT
 */

#ifndef DROP_TRASH_INFO_HPP
#define DROP_TRASH_INFO_HPP

#include <string>

namespace trash_info {

class writer
{
public:
    writer(std::string path_to_be_removed);

    bool write_to(const std::string &dest) const;

private:
    std::string m_path_to_be_removed;
};

} /* namespace trash_info */

#endif /* DROP_TRASH_INFO_HPP */
