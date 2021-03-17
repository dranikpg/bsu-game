#include "camera_system.h"

#include "../components/camera_focus_component.h"
#include "../components/camera_component.h"
#include "../components/position_component.h"

namespace game {

void game::CameraSystem::Run(ecs::World* world) {
  ecs::Entity* camera_entity = world->ScanEntities<PositionComponent, CameraComponent>().Peek();
  ecs::Entity* focus_on_entity = world->ScanEntities<PositionComponent,
                                                     CameraFocusComponent>().Peek();
  if (camera_entity == nullptr || focus_on_entity == nullptr) {
    return;
  }

  QPoint vector = focus_on_entity->GetComponent<PositionComponent>().position -
                  camera_entity->GetComponent<PositionComponent>().position;
  vector *= 0.05;
  camera_entity->GetComponent<PositionComponent>().position += vector;
}
}  // namespace game

