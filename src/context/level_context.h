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
  using LevelRef = std::unique_ptr<resources::Level>;
  using Callback = std::function<void(LevelRef)>;
  void SetLoadCallback(const Callback& load_callback);
  template<typename T>
  void Load();
 private:
  Callback loadCallback_;
};

template<typename T>
void LevelContext::Load() {
  if (loadCallback_) {
    LevelRef ref = std::make_unique<T>();
    loadCallback_(std::move(ref));
  }
}

}  // namespace context

#endif  // SRC_CONTEXT_LEVEL_CONTEXT_H_
