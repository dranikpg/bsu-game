#ifndef SRC_LEVELS_UPPER_FLOOR_CHERNOV_MINI_GAME_H_
#define SRC_LEVELS_UPPER_FLOOR_CHERNOV_MINI_GAME_H_

#include "../../resources/mini_game.h"
#include "../../resources/animation.h"
#include "../../context/input_context.h"
#include "../../widgets/npc_dialog.h"
#include "../../widgets/choose_widget/choose_widget.h"

#include <unordered_map>
#include <string>
#include <memory>

#include <QWidget>
#include <QPixmap>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

namespace game {

class ChernovMiniGame : public resource::MiniGame {
 public:
  class Drawer : public QWidget {
   public:
    using Callback = std::function<void()>;
    explicit Drawer(Callback callback, QWidget* container, ecs::World* world);
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void Process();

   private:
    enum class GameState {
      kDialog,
      kMillionaire,
      kFirst,
      kFirstEnd,
      kEnd
    } game_state_ = GameState::kFirst;

    QWidget* container_;
    ecs::World* world_;
    Callback callback_;

    QWidget* chernov_dialog_container_;
    std::shared_ptr<ui::NPCDialog> chernov_dialog_;
    QWidget* player_dialog_container_;
    std::shared_ptr<ui::NPCDialog> player_dialog_;

    QWidget* millionaire_container_;
    std::shared_ptr<ui::ChooseWidget> millionaire_;
    int chosen_var_;
    std::array<QString, 5> millionaire_pack_;

    QPixmap background_;
    using AnimationBag = std::unordered_map<std::string,
                                            std::shared_ptr<resource::Animation>>;
    AnimationBag chernov_animation_;
    QPixmap chernov_;
    QRect chernov_screen_bounds_ = {0, 0, 0, 0};
    QRect chernov_pixmap_bounds_ = {0, 0, 0, 0};
    ecs::Entity* chernov_player_ = nullptr;
    void PauseChernovPlayer();
    void UnpauseChernovPlayer();
    void MakeChernovSpeaking();
    void MakeChernovNotSpeaking();

    void ComputeChernovBounds();
    QPixmap GetScreenShot();

    void RecalculateSizes();
  };

 public:
  using Callback = std::function<void()>;
  explicit ChernovMiniGame(Callback callback, QWidget* container,
                           ecs::World* world);
  void Process();

 private:
  Drawer* drawer_;
};

}  // namespace game

#endif  // SRC_LEVELS_UPPER_FLOOR_CHERNOV_MINI_GAME_H_
