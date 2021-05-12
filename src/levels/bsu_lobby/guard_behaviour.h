#ifndef SRC_LEVELS_BSU_LOBBY_GUARD_BEHAVIOUR_H_
#define SRC_LEVELS_BSU_LOBBY_GUARD_BEHAVIOUR_H_

#include <memory>

#include <QPointF>

#include "../../ecs/entity.h"
#include "../../resources/path.h"
#include "../../resources/dialog.h"
#include "../../components/behaviour_component.h"
#include "../../utils/pixmap_rect.h"
#include <utility>

namespace game {

class GuardBehaviour : public resource::Behaviour {
 public:
  GuardBehaviour() = default;
  GuardBehaviour(ecs::Entity* player,
                 std::shared_ptr<resource::Path> guard_path,
                 const QPointF& main_position);

  void Process(ecs::Entity* entity) override;
  bool IsDialogFinished() const;

 private:
  void ShowDialog(ecs::Entity* entity);
  float CalculateSpeed(float player_dist, QPointF guard_position);

 private:
  enum class GuardState {
    kNone,
    kGuarding,
    kWandering,
    kDialogFinished
  };

  ecs::Entity* player_ = nullptr;
  std::shared_ptr<resource::Path> guard_path_;
  QPointF main_position_;
  GuardState state_ = GuardState::kNone;

  utils::PixmapRect guard_icon_;
  resource::Dialog dialog_;

  const float kGuardWanderSpeed = 3;
  const float kRunRadius = 170;
  const float kSpeakRadius = 100;
};

}  // namespace game

#endif  // SRC_LEVELS_BSU_LOBBY_GUARD_BEHAVIOUR_H_
