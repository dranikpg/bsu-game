#ifndef SRC_RESOURCES_DIALOG_H_
#define SRC_RESOURCES_DIALOG_H_

#include <QString>

#include <vector>
#include <map>

namespace resource {

class Dialog {
 public:
  using OptionBag = std::map<QString, QString>;

  Dialog() = default;
  explicit Dialog(std::vector<QString> parts);
  Dialog(std::vector<QString> parts, std::map<QString, QString> options);

  const QString& GetPart(int index) const;
  const std::vector<QString>& GetParts() const;
  const std::map<QString, QString>& GetOptions() const;
  int GetStepCount() const;
 private:
  std::vector<QString> parts_;
  std::map<QString, QString> options_;
};

}  // namespace resource

#endif  // SRC_RESOURCES_DIALOG_H_
