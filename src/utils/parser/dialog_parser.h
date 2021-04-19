#ifndef SRC_UTILS_PARSER_DIALOG_PARSER_H_
#define SRC_UTILS_PARSER_DIALOG_PARSER_H_

#include <utility>
#include <stdexcept>

#include <QFile>

#include "../../resources/dialog.h"

namespace utils {

class DialogParser {
 public:
  static resource::Dialog Parse(QFile file);
};

}  // namespace utils

#endif  // SRC_UTILS_PARSER_DIALOG_PARSER_H_
