#ifndef SRC_CONTEXT_LEVEL_CONTEXT_H_
#define SRC_CONTEXT_LEVEL_CONTEXT_H_

#include <memory>
#include <utility>
#include <functional>

#include <QFile>

#include "../resources/level.h"

namespace context {

class LevelContext {
 public:
  using LevelRef = std::unique_ptr<resource::Level>;
  using Listener = std::function<void(LevelRef)>;

  void SetOnLevelCreated(const Listener& listener);

  template<typename T>
  void Load();

 private:
  Listener on_level_created_;
};

template<typename T>
void LevelContext::Load() {
  if (on_level_created_) {
    LevelRef ref = std::make_unique<T>();
    on_level_created_(std::move(ref));
  }
}

}  // namespace context

#endif  // SRC_CONTEXT_LEVEL_CONTEXT_H_
