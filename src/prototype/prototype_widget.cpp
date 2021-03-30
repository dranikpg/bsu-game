#include "prototype_widget.h"

#include <vector>
#include <utility>
#include <memory>

#include <QPainter>
#include <QTimer>
#include <QDebug>

#include "../systems/rendering_system.h"
#include "../systems/animation_system.h"
#include "../systems/movement_system.h"
#include "../systems/input_movement_system.h"
#include "../systems/level_system.h"

#include "../components/sprite_component.h"
#include "../components/impulse_component.h"
#include "../components/animation_component.h"
#include "../components/position_component.h"
#include "../components/camera_component.h"
#include "../components/input_movement_component.h"

#include "../resources/animation.h"

#include "../utils/parser/ase_animation_parser.h"
#include "../map/map_loader.h"

#include "../levels/example/example_level.h"

PrototypeWidget::PrototypeWidget() {
  std::vector<std::unique_ptr<System>> systems;
  systems.emplace_back(std::make_unique<game::LevelSystem>(&level_context_));
  systems.emplace_back(
      std::make_unique<game::RenderingSystem>(&painter_context_, &window_context_));
  systems.emplace_back(std::make_unique<game::AnimationSystem>());
  systems.emplace_back(std::make_unique<game::InputMovementSystem>(&input_context_));
  systems.emplace_back(std::make_unique<game::MovementSystem>());
  world_.Init(std::move(systems));

  // Load example level
  level_context_.Load<level::ExampleLevel>();

  connect(&timer_, &QTimer::timeout, [this]() {
    update();
  });
  timer_.start(16);
}

void PrototypeWidget::paintEvent(QPaintEvent* event) {
  QWidget::paintEvent(event);

  QPainter painter(this);
  painter_context_.SetPainter(&painter);
  window_context_.SetSize(size());

  world_.Run();
}

void PrototypeWidget::keyPressEvent(QKeyEvent* event) {
  input_context_.AddKey(static_cast<Qt::Key>(event->key()));
}

void PrototypeWidget::keyReleaseEvent(QKeyEvent* event) {
  input_context_.RemoveKey(static_cast<Qt::Key>(event->key()));
}
