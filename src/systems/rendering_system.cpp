#include "rendering_system.h"

#include <QRectF>
#include <QPointF>
#include <QPainter>

#include <algorithm>

#include "../components/position_component.h"
#include "../components/sprite_component.h"
#include "../components/camera_component.h"
#include "../constants/sprite_layer.h"

using constants::SpriteLayer;

namespace game {

RenderingSystem::RenderingSystem(PainterContext* painter_context,
                                 WindowContext* window_context)
    : painter_context_(painter_context), window_context_(window_context) {}

void RenderingSystem::Run(World* world) {
  QPainter& painter = painter_context_->GetPainter();
  painter.save();

  ecs::Entity* camera_entity =
      world->ScanEntities<PositionComponent, CameraComponent>().Peek();
  if (camera_entity) {
    auto[camera, camera_pos] = camera_entity->Unpack<CameraComponent, PositionComponent>();
    float scale = camera.scale;
    painter.scale(scale, scale);
    painter.translate(-camera_pos.position);
    painter.translate(window_context_->GetSize().width() / scale / 2,
                      window_context_->GetSize().height() / scale / 2);
    window_context_->SetRenderTransform(painter.transform());
  }

  std::vector<ecs::Entity*> current_layer;

  for (int layer_id = 0;
       SpriteLayer::_LAST != static_cast<SpriteLayer>(layer_id); layer_id++) {
    auto layer = static_cast<SpriteLayer>(layer_id);
    for (auto& entity : world->ScanEntities<SpriteComponent,
                                            PositionComponent>()) {
      if (entity.GetComponent<SpriteComponent>().layer == layer) {
        current_layer.push_back(&entity);
      }
    }
    std::sort(current_layer.begin(), current_layer.end(), EntityYSort);
    for (auto& entity: current_layer) {
      auto[sprite, position] = entity->Unpack<SpriteComponent,
                                              PositionComponent>();
      QRectF bounds = sprite.target_area;
      bounds.moveCenter(position.position);
      bounds.translate(sprite.target_area.topLeft());
      painter.drawPixmap(bounds, sprite.pixmap, sprite.source_area);
    }
    current_layer.clear();
  }

  painter.restore();
}

bool RenderingSystem::EntityYSort(const ecs::Entity* l, const ecs::Entity* r) {
  return l->GetComponent<PositionComponent>().position.y() <
      r->GetComponent<PositionComponent>().position.y();
}

}  // namespace game
