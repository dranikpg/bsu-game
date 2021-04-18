#ifndef SRC_RESOURCES_DIALOG_H_
#define SRC_RESOURCES_DIALOG_H_

#include <QString>

#include <vector>
#include <map>

namespace resource {

class Dialog {
 public:
  Dialog(std::vector<QString> parts, std::map<QString, QString> answers);
  const QString& GetPart(int index) const;
  const std::vector<QString>& GetParts() const;
  const std::map<QString, QString>& GetAnswers() const;
  int GetStepCount() const;
 private:
  std::vector<QString> parts_;
  std::map<QString, QString> answers_;
};

}  // namespace resource

#endif  // SRC_RESOURCES_DIALOG_H_
