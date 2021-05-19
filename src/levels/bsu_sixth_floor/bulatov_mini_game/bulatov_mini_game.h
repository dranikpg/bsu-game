#ifndef bla
#define bla

#include "../../../resources/mini_game.h"
#include "../../../resources/animation.h"
#include "../../../context/input_context.h"
#include "answer_widget.h"

#include <unordered_map>

#include <QWidget>
#include <QPixmap>
#include <QLineEdit>
#include <QPushButton>

namespace game {

class BulatovMiniGame : public resource::MiniGame {
 public:
  class Drawer : public QWidget {
   public:
    explicit Drawer(QWidget* container, ecs::World* world);
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
   private:
    enum class GameState {
      k
    };
    QWidget* container_;
    ecs::World* world_;

    using AnimationBag = std::unordered_map<std::string,
                                            std::shared_ptr<resource::Animation>>;
    AnimationBag animations_;
    ecs::Entity* animation_player;
    QRect current_frame_bounds_;
    QPixmap background_;
    AnswerWidget* answer_widget_;

    void LoadAnimations();
    void LoadFrameBounds();
  };

 public:
  using Callback = std::function<void()>;
  explicit BulatovMiniGame(Callback callback, QWidget* container,
                           ecs::World* world);
  void Process();

 private:
  Drawer* drawer_;
  Callback callback_;
};

}  // namespace game

#endif  // SRC_LEVELS_BSU_LOBBY_CANTEEN_MINI_GAME_H_
