#ifndef SRC_LEVELS_BSU_LOBBY_CANTEEN_MINI_GAME_H_
#define SRC_LEVELS_BSU_LOBBY_CANTEEN_MINI_GAME_H_

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

  class CanteenDrawer : public QWidget {
   public:
    CanteenDrawer(QWidget* container,
                  std::shared_ptr<std::map<PlayerColumn, std::vector<double>>> opa);
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    double GetCoordinate(const PlayerColumn& state);

    std::shared_ptr<PlayerColumn> player_state_ = nullptr;
   private:
    QWidget* container_ = nullptr;
    std::shared_ptr<std::map<PlayerColumn, std::vector<double>>> opa_;
    QPixmap background_ = QPixmap(":/canteen.png");
    QPixmap chiabata_ = QPixmap(":/chiabatta.png");
    QPixmap chel_ = QPixmap(":/chel_for_bufet");
    std::vector<QPixmap> levels_ = {QPixmap(":/bufet_levels1.png"),
                                   QPixmap(":/bufet_levels2.png"),
                                   QPixmap(":/bufet_levels3.png"),
                                   QPixmap(":/bufet_levels4.png"),
                                   QPixmap(":/bufet_levels5.png")};
    std::vector<QPixmap> hearts_ = {QPixmap(":/hearts3.png"),
                                   QPixmap(":/hearts2.png"),
                                   QPixmap(":/hearts1.png")};
  };

 public:
  using Callback = std::function<void()>;
  CanteenMiniGame(Callback callback, QWidget* container, context::InputContext* input);
  void Process();
  void EraseChiabattas(const std::set<std::pair<PlayerColumn, double>>& for_erasing);
  void ProccesChiabattas(std::set<std::pair<PlayerColumn, double>>* for_erasing);
  void CollidingChiabattas(std::set<std::pair<PlayerColumn, double>>* for_erasing);

  static int timer;
  static int curr_level;
  static int lifes;
  static State state;

 private:
  void AddChiabata(int rand);
  CanteenDrawer* canteen_;
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
  QRect chel_size_ = QPixmap(":/chel_for_bufet.png").rect();
  QRect chia_size_ = QPixmap(":/chiabatta.png").rect();
};

}  // namespace game

#endif  // SRC_LEVELS_BSU_LOBBY_CANTEEN_MINI_GAME_H_
