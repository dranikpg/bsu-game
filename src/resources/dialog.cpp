#include "dialog.h"

#include <utility>

namespace resource {

Dialog::Dialog(std::vector<QString> parts,
               std::map<QString, QString> answers)
    : parts_(std::move(parts)), answers_(std::move(answers)) {}

const std::vector<QString>& Dialog::GetParts() const {
  return parts_;
}

const std::map<QString, QString>& Dialog::GetAnswers() const {
  return answers_;
}

int Dialog::GetStepCount() const {
  return parts_.size() + (!answers_.empty());
}

const QString& Dialog::GetPart(int index) const {
  return parts_[index];
}

}  // namespace resource
