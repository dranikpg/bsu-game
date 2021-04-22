#ifndef SRC_LEVELS_BSU_ENTRANCE_GUARD_MINI_GAME_H_
#define SRC_LEVELS_BSU_ENTRANCE_GUARD_MINI_GAME_H_

#include "../../resources/mini_game.h"

namespace game {

class GuardMiniGame : public resource::MiniGame {
 public:
  using Callback = std::function<void()>;

  explicit GuardMiniGame(const Callback& callback);
  void Process();

 private:
  int f = 0;
  Callback callback_;
};

}  // namespace game

#endif  // SRC_LEVELS_BSU_ENTRANCE_GUARD_MINI_GAME_H_
