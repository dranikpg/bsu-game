#include "prototype_widget.h"

#include <vector>
#include <utility>
#include <memory>

#include <QPainter>
#include <QPixmap>

#include "../components/sprite_component.h"
#include "../components/position_component.h"
#include "../components/camera_component.h"

PrototypeWidget::PrototypeWidget() {
  std::vector<std::unique_ptr<System>> systems;
  systems.emplace_back(std::make_unique<game::RenderingSystem>(&painter_context_));
  world_.Init(std::move(systems));

  // TEST EXAMPLE
  /*
  QPixmap dragon;
  dragon.load("../test/dragon.jpg");
  world_.CreateEntity()
      .AddComponent<game::PositionComponent>(0, 0)
      .AddComponent<game::SpriteComponent>(dragon);

  world_.CreateEntity()
      .AddComponent<game::PositionComponent>(-50,-50)
      .AddComponent<game::CameraComponent>();
  */
}

void PrototypeWidget::paintEvent(QPaintEvent* event) {
  QWidget::paintEvent(event);

  QPainter painter(this);
  painter_context_.Supply(&painter);

  world_.Run();
}
