#ifndef SRC_LEVELS_LABYRINTH_LABYRINTH_MINI_GAME_H_
#define SRC_LEVELS_LABYRINTH_LABYRINTH_MINI_GAME_H_

#include "../../resources/mini_game.h"
#include "../../context/input_context.h"

#include <QWidget>
#include <QPointF>
#include <map>
#include <memory>
#include <vector>
#include <set>
#include <utility>
#include <QPixmap>
#include <QMediaPlaylist>
#include <QMediaPlayer>

namespace game {

class LabyrinthMiniGame : public resource::MiniGame {
 public:
  enum class State {
    kNotStarted,
    kWaiting,
    kGame,
    kGameOver,
    kGameEnding
  };

  class Drawer : public QWidget {
   public:
    explicit Drawer(LabyrinthMiniGame*);
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

   private:
    LabyrinthMiniGame* mini_game_;
    QRect rect_radial;
    QRadialGradient radialGrad;
  };
  friend class Drawer;

 public:
  using Callback = std::function<void()>;
  LabyrinthMiniGame(Callback callback, QWidget* container, context::InputContext* input,
                    std::shared_ptr<bool> is_switched);
  void Process(QPointF player_pos);

 private:
  Drawer* drawer_;
  Callback callback_;
  QWidget* container_ = nullptr;
  context::InputContext* input_ = nullptr;
  QPointF player_pos_ = QPointF(0, 0);
  std::shared_ptr<bool> is_sw_;
};

}  // namespace game

#endif  // SRC_LEVELS_LABYRINTH_LABYRINTH_MINI_GAME_H_
