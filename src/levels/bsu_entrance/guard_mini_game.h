#ifndef SRC_LEVELS_BSU_ENTRANCE_GUARD_MINI_GAME_H_
#define SRC_LEVELS_BSU_ENTRANCE_GUARD_MINI_GAME_H_

#include "../../resources/mini_game.h"

#include <QWidget>
#include <QPointF>

namespace game {

class GuardMiniGame : public resource::MiniGame {
 public:
  class NimbusDrawer : public QWidget {
   public:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void SetPlayerPos(const QPointF& player_pos);
   private:
    QPointF player_pos_;
  };


 public:
  using Callback = std::function<void()>;
  explicit GuardMiniGame(const Callback& callback, QWidget* container);
  void Process(QPointF player_pos);
 private:
  int timer_ = 0;
  NimbusDrawer* nimbus_;
  Callback callback_;
};

}  // namespace game

#endif  // SRC_LEVELS_BSU_ENTRANCE_GUARD_MINI_GAME_H_
