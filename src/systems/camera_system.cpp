#include "camera_system.h"

#include <algorithm>

#include <QPointF>
#include <QRectF>

#include "../ecs/entity.h"
#include "../components/position_component.h"
#include "../components/bounds_component.h"
#include "../components/camera_component.h"
#include "../components/camera_follow_component.h"
#include "../components/map_component.h"

namespace game {

void CameraSystem::Run(ecs::World* world) {
  ecs::Entity* camera = world->ScanEntities<PositionComponent, CameraComponent>().Peek();
  ecs::Entity* target = world->ScanEntities<PositionComponent, CameraFollowComponent>().Peek();
  ecs::Entity* map = world->ScanEntities<PositionComponent, BoundsComponent, MapComponent>().Peek();

  if (camera == nullptr || target == nullptr || map == nullptr) {
    return;
  }

  auto& target_pos = target->GetComponent<PositionComponent>();
  auto[camera_pos, camera_info] = camera->Unpack<PositionComponent, CameraComponent>();
  auto[map_pos, map_bounds] = map->Unpack<PositionComponent, BoundsComponent>();

  // difvv
  QPointF dif_vector = target_pos.position - camera_pos.position;
  camera_pos.position += kLerpFactor * dif_vector;

  QRectF map_rect = QRectF(QPointF(0, 0), map_bounds.bounds);
  map_rect.moveCenter(map_pos.position);
  QSizeF window_size = QSizeF(window_context_->GetSize().width(),
                              window_context_->GetSize().height());

  float minScale = std::max(window_size.width() / map_bounds.bounds.width(),
                            window_size.height() / map_bounds.bounds.height());
  camera_info.scale = std::max(camera_info.targetScale, minScale);

  QRectF camera_rect = QRectF(QPointF(0, 0), window_size / camera_info.scale);
  camera_rect.moveCenter(camera_pos.position);

  if (camera_rect.left() < map_rect.left()) {
    camera_rect.moveLeft(map_rect.left());
  }
  if (camera_rect.right() > map_rect.right()) {
    camera_rect.moveRight(map_rect.right());
  }
  if (camera_rect.top() < map_rect.top()) {
    camera_rect.moveTop(map_rect.top());
  }
  if (camera_rect.bottom() > map_rect.bottom()) {
    camera_rect.moveBottom(map_rect.bottom());
  }

  camera_pos.position = camera_rect.center();
}

CameraSystem::CameraSystem(context::WindowContext* window_context)
    : window_context_(window_context) {}

}  // namespace game
