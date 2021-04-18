#ifndef SRC_CONTEXT_INPUT_CONTEXT_H_
#define SRC_CONTEXT_INPUT_CONTEXT_H_

#include <set>
#include <optional>

#include <QKeyEvent>

#include "../constants/keys.h"

using constants::Keys;

namespace context {

class InputContext {
 public:
  void AddKey(Qt::Key key);
  void RemoveKey(Qt::Key key);
  const std::set<constants::Keys>& GetKeys() const;
  std::set<constants::Keys>& GetFrameKeys();
  void Clean();

 private:
  static std::optional<Keys> MapKey(Qt::Key key);
  std::set<constants::Keys> keys_;
  std::set<constants::Keys> frame_keys_;
};
}  // namespace context

#endif  // SRC_CONTEXT_INPUT_CONTEXT_H_
