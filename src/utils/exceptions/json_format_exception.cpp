#include "json_format_exception.h"

utils::JsonFormatException::JsonFormatException(const std::string& string)
    : runtime_error(string) {}
