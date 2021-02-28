#ifndef SRC_CONTEXT_INPUT_CONTEXT_H_
#define SRC_CONTEXT_INPUT_CONTEXT_H_

#include <set>
#include <QKeyEvent>

#include "../constants/keys.h"

using constants::Keys;

namespace context {

class InputContext {
 public:
  void SetKeys(Qt::Key key);
  std::set<constants::Keys>& GetKeys();
  void Clean();

 private:
  std::set<constants::Keys> seq_of_keys_{};
};
}  // namespace context
#endif  // SRC_CONTEXT_INPUT_CONTEXT_H_
