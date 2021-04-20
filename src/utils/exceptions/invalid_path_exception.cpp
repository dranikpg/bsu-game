#include "invalid_path_exception.h"

utils::InvalidPathException::InvalidPathException(const QFile& file)
  : std::runtime_error("File not found " + file.fileName().toStdString()) {}
