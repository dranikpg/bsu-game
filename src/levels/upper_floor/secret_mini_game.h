#ifndef SRC_LEVELS_BSU_ENTRANCE_SECRET_MINI_GAME_H_
#define SRC_LEVELS_BSU_ENTRANCE_SECRET_MINI_GAME_H_

#include <memory>
#include <vector>
#include <unordered_map>
#include <string>
#include <utility>
#include <QMouseEvent>
#include <QMediaPlayer>
#include <QMediaPlaylist>



#include "../../resources/mini_game.h"
#include "../../context/input_context.h"
#include "../../ecs/entity.h"
#include "../../ecs/world.h"
#include "../../utils/parser/ase_animation_parser.h"

namespace game {

class SecretMiniGame {
 public:
  enum class GameState {
    kWaiting,
    kThrowing,
    kReturningGood,
    kReturningBad,
    kGoodEnding,
    kBadEnding
  };

  class Drawer : public QWidget {
   public:
    friend class SecretMiniGame;
    explicit Drawer(SecretMiniGame* game);
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent* event) override;

    QPointF click_pos;
    bool is_changed_click = false;

   private:
    SecretMiniGame* mini_game;

    QMediaPlayer* media_player_;
    QMediaPlaylist* first_playlist_;
    QMediaPlaylist* second_playlist_;
    QMediaPlaylist* third_playlist_;

    QPixmap bad_ = QPixmap(":/tablichki1.png");
    QPixmap good_ = QPixmap(":/tablichki2.png");
    QPixmap player_pixmap_ = QPixmap(":/chel_with_labs.png");
    QPixmap background_pixmap_ = QPixmap(":/computer_class_fon1");;
    QPixmap wa_pixmap_ = QPixmap(":/WA.png");
    QPixmap ok_pixmap_ = QPixmap(":/OK.png");
    QPixmap lab_pixmap_ = QPixmap(":/laba.png");
    QPixmap target_pixmap_ = QPixmap(":/target.png");
    std::vector<QPixmap> hearts_pixmap_ = {QPixmap(":/hearts3.png"),
                                           QPixmap(":/hearts2.png"),
                                           QPixmap(":/hearts1.png")};

    int count_ = 0;
    bool stop_ = false;
    bool changed_side_ = false;
    bool changed_playlist_ = false;
    bool started_ = false;
  };

  using Callback = std::function<void()>;
  SecretMiniGame(Callback callback, QWidget* container, context::InputContext* input, ecs::World*);
  void Process();

 private:
  struct Player {
    explicit Player(ecs::World* world);

    ecs::Entity* player = nullptr;
    GameState state = GameState::kWaiting;
    QPointF coordinate;
    int lives = 3;
    int hits = 0;
  };

  struct Target {
    explicit Target(ecs::World* world);

    ecs::Entity* target = nullptr;
    QPointF coordinate;
    int speed;
    int side = 1;
    bool is_hit = false;
    bool is_running = false;
    std::unordered_map<std::string, std::shared_ptr<Animation>> anims_;
  };

  struct Lab {
    Lab();

    const int kSpeed;
    QPointF coordinate;
    QPointF curr_vector;
  };


  Player* player_ = nullptr;
  Target* target_ = nullptr;
  Lab* lab_ = nullptr;

  Callback callback_;
  context::InputContext* input_ = nullptr;
  Drawer* drawer_ = nullptr;
  QWidget* container_ = nullptr;

  int timer_ = 0;
  int kWaitingTime_;

  QPointF for_updating_;
  bool clicked_ = false;
};

}  // namespace game
#endif  // SRC_LEVELS_BSU_ENTRANCE_SECRET_MINI_GAME_H_
