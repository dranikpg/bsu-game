#ifndef SRC_LEVELS_BSU_LOBBY_CANTEEN_MINI_GAME_H_
#define SRC_LEVELS_BSU_LOBBY_CANTEEN_MINI_GAME_H_

#include "../../resources/mini_game.h"
#include "../../context/input_context.h"

#include <QWidget>
#include <QPointF>
#include <map>
#include <memory>
#include <vector>

namespace game {

class CanteenMiniGame : public resource::MiniGame {
 private:
  enum class PlayerColumn {
    kFist,
    kSecond,
    kThird,
    kFourth
  };

 public:
  enum class State {
    kWaiting,
    kGame,
    kGameOver,
    kGameEnding
  };

  class NimbusDrawer : public QWidget {
   public:
    NimbusDrawer(QWidget* container, std::shared_ptr<std::map<PlayerColumn, std::vector<double>>> opa);
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void SetPlayerPos(const QPointF& player_pos);
    double GetCoordinate(const PlayerColumn& state);

    std::shared_ptr<PlayerColumn> player_state_ = nullptr;
   private:
    QWidget* container_ = nullptr;
    std::shared_ptr<std::map<PlayerColumn, std::vector<double>>> opa_;
  };

 public:
  using Callback = std::function<void()>;
  CanteenMiniGame(Callback callback, QWidget* container, context::InputContext* input);
  void Process();
  void EraseChiabattas(const std::set<std::pair<PlayerColumn, double>>& for_erasing);
  void ProccesChiabattas(std::set<std::pair<PlayerColumn, double>>& for_erasing);
  void CollidingChiabattas(std::set<std::pair<PlayerColumn, double>>& for_erasing);

  static int timer;
  static int curr_level;
  static int lifes;
  static State state;

 private:
  void AddChiabata(int rand);
  NimbusDrawer* nimbus_;
  Callback callback_;
  std::shared_ptr<std::map<PlayerColumn, std::vector<double>>> chiabatas_;
  QWidget* container_ = nullptr;
  std::shared_ptr<PlayerColumn> chel_ty = nullptr;
  context::InputContext* input_ = nullptr;
  const int kFirstSpeed = 70;
  const int kSecondSpeed = 50;
  const int kUnreal = 30;
  int curr_speed_;
  int count_chia_ = 0;
  int curr_num_chia_ = 0;
};

}  // namespace game

#endif  // SRC_LEVELS_BSU_LOBBY_CANTEEN_MINI_GAME_H_
