#include "rendering_system.h"

#include <QPainter>

#include "../components/position_component.h"
#include "../components/sprite_component.h"
#include "../components/camera_component.h"
#include "../constants/sprite_layer.h"

using constants::SpriteLayer;

namespace game {

RenderingSystem::RenderingSystem(PainterContext* painter_context, WindowContext* window_context)
    : painter_context_(painter_context), window_context_(window_context) {}

void RenderingSystem::Run(World* world) {
  QPainter& painter = painter_context_->GetPainter();
  painter.save();

  ecs::Entity* camera_entity = world->ScanEntities<PositionComponent, CameraComponent>().Peek();
  if (camera_entity) {
    auto& camera_pos = camera_entity->GetComponent<PositionComponent>();
    painter.translate(
        -camera_pos.position.x() + window_context_->GetSize().width() / 2,
        -camera_pos.position.y() + window_context_->GetSize().height() / 2
    );
  }

  for (int layer_id = 0; SpriteLayer::_LAST != static_cast<SpriteLayer>(layer_id); layer_id++) {
    auto layer = static_cast<SpriteLayer>(layer_id);
    for (auto[sprite, position] : world->Scan<SpriteComponent, PositionComponent>()) {
      if (sprite.layer != layer) {
        continue;
      }
      QRect bounds = sprite.target_area;
      bounds.moveCenter(position.position);
      painter.drawPixmap(bounds, sprite.pixmap, sprite.source_area);
    }
  }

  painter.restore();
}

}  // namespace game
