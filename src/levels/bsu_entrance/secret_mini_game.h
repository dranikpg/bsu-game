#ifndef SRC_LEVELS_BSU_ENTRANCE_SECRET_MINI_GAME_H_
#define SRC_LEVELS_BSU_ENTRANCE_SECRET_MINI_GAME_H_

#include <memory>
#include <vector>
#include <unordered_map>
#include <string>
#include <utility>
#include <QMouseEvent>

#include "../../resources/mini_game.h"
#include "../../context/input_context.h"
#include "../../ecs/entity.h"
#include "../../ecs/world.h"
#include "../../utils/parser/ase_animation_parser.h"

namespace game {

class SecretMiniGame {
 public:
  enum class PudgeState {
    kWaiting,
    kHooking,
    kReturningGood,
    kReturningBad,
    kGoodEnding,
    kBadEnding
  };

  class Drawer : public QWidget {
   public:
    friend class SecretMiniGame;
    Drawer(QWidget* container, ecs::World* world);
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent* event) override;

    static QPointF click_pos;
    bool is_changed = false;

   private:
    QWidget* container_ = nullptr;
    QPixmap pudge_pixmap_ = QPixmap(":/chel_with_labs.png");
    QPixmap background_ = QPixmap(":/computer_class_fon1");;
    QPixmap wa_ = QPixmap(":/WA.png");
    QPixmap ok_ = QPixmap(":/OK.png");
    QPixmap hook = QPixmap(":/laba.png");
    std::vector<QPixmap> hearts_ = {QPixmap(":/hearts3.png"),
                                    QPixmap(":/hearts2.png"),
                                    QPixmap(":/hearts1.png")};
    std::unordered_map<std::string, std::shared_ptr<Animation>> anims_;
    int16_t count_ = 0;
    bool stop_ = false;
    ecs::Entity* pudge_ = nullptr;
    ecs::Entity* target = nullptr;
  };

  using Callback = std::function<void()>;
  SecretMiniGame(Callback callback, QWidget* container, context::InputContext* input, ecs::World*);
  void Process();

  static PudgeState pudge_state;
  static QPointF hook_coordinates;
  static QPointF pudge;
  static bool clicked;
  static QPointF curr_vector;
  static std::vector<std::pair<QPointF, bool>> coordinates;
  static int lifes;

 private:
  Callback callback_;
  Drawer* drawer_;
  QWidget* container_ = nullptr;
  context::InputContext* input_ = nullptr;
  ecs::World* world_;
  QPointF for_updating_;
  int hits_ = 0;
  int timer_ = 0;
};

}  // namespace game
#endif  // SRC_LEVELS_BSU_ENTRANCE_SECRET_MINI_GAME_H_
