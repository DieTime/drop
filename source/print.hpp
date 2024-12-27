/**
 * SPDX-FileCopyrightText: 2024 Denis Glazkov <glazzk.off@mail.ru>
 * SPDX-License-Identifier: MIT
 */

#ifndef DROP_LOG_HPP
#define DROP_LOG_HPP

#include <string>

namespace print {

enum class verbose_mode { enabled, disabled };
void set_verbose_mode(verbose_mode mode);

void error(const std::string &message);
void oops(const std::string &message, const std::string &description);
void message(const std::string &message);
void verbose(const std::string &message, const std::string &description);

} /* namespace print */

#endif /* DROP_LOG_HPP */
