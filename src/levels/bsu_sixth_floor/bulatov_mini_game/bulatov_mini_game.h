#ifndef bla
#define bla

#include "../../../resources/mini_game.h"
#include "../../../resources/animation.h"
#include "../../../context/input_context.h"
#include "../../../widgets/npc_dialog.h"
#include "../../../widgets/choose_widget/choose_widget.h"

#include <unordered_map>

#include <QWidget>
#include <QPixmap>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

namespace game {

class BulatovMiniGame : public resource::MiniGame {
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
      kStartup,
      kQ1,
      kQ1No,
      kQ1Yes,
      kQ1NoQ2No,
      kQ1NoQ2Yes,
      kQ1YesQ2Yes,
      kQ1YesQ2No,
      kProcessing,
      kEnd
    } game_state_ = GameState::kStartup,
      real_state_ = GameState::kStartup;
    QWidget* container_;
    ecs::World* world_;

    using AnimationBag = std::unordered_map<std::string,
                                            std::shared_ptr<resource::Animation>>;
    AnimationBag animations_;
    ecs::Entity* animation_player;
    QRect current_frame_bounds_;
    QPixmap background_;
    ui::NPCDialog* bulatov_dialog_widget_;
    QWidget* bulatov_dialog_container_;
    QWidget* player_dialog_container_;
    QWidget* choose_widget_container_;
    ui::ChooseWidget* choose_widget_ = nullptr;
    int choose_widget_return_ = -1;

    bool dialog_finished_ = false;
    Callback callback_;

    void LoadAnimations();
    void RecalculateSizes();

    void MakeSpeaking();
    void MakeNotSpeaking();
  };

 public:
  using Callback = std::function<void()>;
  explicit BulatovMiniGame(Callback callback, QWidget* container,
                           ecs::World* world);
  void Process();
 private:
  Drawer* drawer_;
};

}  // namespace game

#endif  // SRC_LEVELS_BSU_LOBBY_CANTEEN_MINI_GAME_H_
