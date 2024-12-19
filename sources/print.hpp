#ifndef DROP_LOG_HPP
#define DROP_LOG_HPP

#include <string>

namespace print {

void error(const std::string &message);
void oops(const std::string &message, const std::string &description);
void message(const std::string &message);

} /* namespace print */

#endif /* DROP_LOG_HPP */
