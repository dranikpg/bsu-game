#ifndef SRC_UTILS_EXCEPTIONS_INVALID_PATH_EXCEPTION_H_
#define SRC_UTILS_EXCEPTIONS_INVALID_PATH_EXCEPTION_H_

#include <string>
#include <stdexcept>

#include <QFile>

namespace utils {

class InvalidPathException : public std::runtime_error {
 public:
  explicit InvalidPathException(const QFile& file);
};

}  // namespace utils

#endif  // SRC_UTILS_EXCEPTIONS_INVALID_PATH_EXCEPTION_H_
