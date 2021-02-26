#include "rendering_system.h"

#include <QPainter>

#include "../ecs/entity.h"

#include "../components/position_component.h"
#include "../components/sprite_component.h"
#include "../components/camera_component.h"
#include "../constants/sprite_layer.h"

using constants::SpriteLayer;

namespace game {

RenderingSystem::RenderingSystem(PainterContext* painter_context)
    : painter_context_(painter_context) {}

void RenderingSystem::Run(World* world) {
  QPainter& painter = painter_context_->Acquire();
  painter.save();

  ecs::Entity* camera_entity = world->ScanEntities<PositionComponent, CameraComponent>().Peek();
  if (camera_entity) {
    auto& camera_pos = camera_entity->GetComponent<PositionComponent>();
    painter.translate(-camera_pos.pos);
  }

  for (int layer_id = 0; SpriteLayer::_LAST != static_cast<SpriteLayer>(layer_id); layer_id++) {
    auto layer = static_cast<SpriteLayer>(layer_id);
    for (auto[sprite, position] : world->Scan<SpriteComponent, PositionComponent>()) {
      if (sprite.layer != layer) {
        continue;
      }
      QRect bounds = sprite.bounds.translated(position.pos);
      bounds.translate(-bounds.width() / 2, -bounds.height() / 2);
      painter.drawPixmap(bounds, sprite.pixmap, sprite.area);
    }
  }

  painter.restore();
}

}  // namespace game
