#include "dialog.h"

#include <utility>

namespace resource {

Dialog::Dialog(std::vector<QString> parts,
               std::map<QString, QString> options)
    : parts_(std::move(parts)), options_(std::move(options)) {}

Dialog::Dialog(std::vector<QString> parts) : parts_(std::move(parts)) {}

const std::vector<QString>& Dialog::GetParts() const {
  return parts_;
}

const std::map<QString, QString>& Dialog::GetOptions() const {
  return options_;
}

int Dialog::GetStepCount() const {
  return parts_.size() + (!options_.empty());
}

const QString& Dialog::GetPart(int index) const {
  return parts_[index];
}

}  // namespace resource
