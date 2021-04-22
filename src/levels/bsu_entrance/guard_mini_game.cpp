#include "guard_mini_game.h"

namespace game {

void game::GuardMiniGame::Process() {
  if (f++ > 120) {
    callback_();
  }
}

game::GuardMiniGame::GuardMiniGame(const game::GuardMiniGame::Callback& callback)
    : callback_(callback) {}

}  // namespace game
