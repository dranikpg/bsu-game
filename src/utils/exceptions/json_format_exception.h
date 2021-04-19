#ifndef SRC_UTILS_EXCEPTIONS_JSON_FORMAT_EXCEPTION_H_
#define SRC_UTILS_EXCEPTIONS_JSON_FORMAT_EXCEPTION_H_

#include <string>
#include <stdexcept>

namespace utils {

class JsonFormatException : public std::runtime_error {
 public:
  explicit JsonFormatException(const std::string& string);
};

}  // namespace utils

#endif  // SRC_UTILS_EXCEPTIONS_JSON_FORMAT_EXCEPTION_H_
