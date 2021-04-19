#include "guard_behaviour.h"

#include <optional>
#include <cmath>
#include <utility>
#include <memory>

#include <QString>
#include <QDebug>

#include "../../constants/path_follow.h"
#include "../../resources/path.h"
#include "../../components/dialog_component.h"
#include "../../components/position_component.h"
#include "../../components/path_follow_component.h"
#include "../../utils/parser/dialog_parser.h"

namespace game {

GuardBehaviour::GuardBehaviour(ecs::Entity* player,
                               std::shared_ptr<resource::Path> guard_path,
                               const QPointF& main_position)
    : player_(player),
      guard_path_(std::move(guard_path)),
      main_position_(main_position) {

  QPixmap guard_sheet(":/guard-sheet.png");
  guard_icon_ = utils::PixRect(guard_sheet, QRect(0, 0, 64, 64));

  dialog_ = utils::DialogParser::Parse(QFile(":/guard_dialog.json"));
}

void GuardBehaviour::Process(ecs::Entity* entity) {
  QPointF player_vector =
      main_position_ - player_->GetComponent<PositionComponent>().position;
  float player_dist = std::hypotf(player_vector.x(), player_vector.y());
  auto& path = entity->GetComponent<PathFollowComponent>();
  if (player_dist < kRunRadius && state_ != GuardState::kGuarding) {
    qDebug() << "switching guard to guarding ";
    state_ = GuardState::kGuarding;
    path = PathFollowComponent(
        resource::Path(main_position_),
        constants::PathFollowType::kOnce,
        player_dist / 5.0f * 0.7f);
  } else if (player_dist > kRunRadius && state_ != GuardState::kWandering) {
    qDebug() << "switching guard to wandering ";
    state_ = GuardState::kWandering;
    path = PathFollowComponent(
        *guard_path_,
        constants::PathFollowType::kClosedLoop,
        kGuardWanderSpeed);
  }

  if (player_dist < kSpeakRadius) {
    ShowDialog(entity);
  } else {
    if (entity->HasComponent<DialogComponent>()) {
      entity->GetComponent<DialogComponent>().Finish();
    }
  }
}

void GuardBehaviour::ShowDialog(ecs::Entity* entity) {
  auto ds = std::make_shared<resource::Dialog>(dialog_);
  if (!entity->HasComponent<DialogComponent>()) {
    entity->AddComponent<DialogComponent>(ds,
                                          [](std::optional<QString>) {
                                            qDebug() << "finished";
                                          }, guard_icon_);
  }
}

}  // namespace game
