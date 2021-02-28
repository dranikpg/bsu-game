#ifndef SRC_CONTEXT_INPUT_CONTEXT_H_
#define SRC_CONTEXT_INPUT_CONTEXT_H_

#include <set>
#include <QObject>
#include <QKeyEvent>
#include <QMainWindow>

#include "../constants/keys.h"

using constants::Keys;

namespace context {

class InputContext : public QWidget {
 public:
  void keyPressEvent(QKeyEvent* event) override;
  std::set<constants::Keys>& GetKeys();
  void Clean();

 private:
  std::set<constants::Keys> seq_of_keys_{};
};
}  // namespace context
#endif  // SRC_CONTEXT_INPUT_CONTEXT_H_
