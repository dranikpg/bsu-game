#include "prototype_widget.h"

#include <vector>
#include <utility>
#include <memory>

#include <QPainter>
#include <QFile>
#include <QTimer>
#include <QDebug>

#include "../systems/rendering_system.h"
#include "../systems/animation_system.h"
#include "../systems/movement_system.h"
#include "../systems/input_movement_system.h"

#include "../components/sprite_component.h"
#include "../components/impulse_component.h"
#include "../components/animation_component.h"
#include "../components/position_component.h"
#include "../components/camera_component.h"
#include "../components/input_movement_component.h"

#include "../resources/animation.h"
#include "../utils/parser/ase_animation_parser.h"

PrototypeWidget::PrototypeWidget() {
  std::vector<std::unique_ptr<System>> systems;
  systems.emplace_back(
      std::make_unique<game::RenderingSystem>(&painter_context_, &window_context_));
  systems.emplace_back(std::make_unique<game::AnimationSystem>());
  systems.emplace_back(std::make_unique<game::InputMovementSystem>(&input_context_));
  systems.emplace_back(std::make_unique<game::MovementSystem>());
  world_.Init(std::move(systems));

  // YOUR_CODE_HERE

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

  input_context_.Clean();
}

void PrototypeWidget::keyPressEvent(QKeyEvent* event) {
  input_context_.AddKey(static_cast<Qt::Key>(event->key()));
}
