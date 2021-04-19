#include "prototype_widget.h"

#include <vector>
#include <utility>
#include <memory>
#include <map>

#include <QPainter>
#include <QTimer>
#include <QDebug>

#include "../systems/rendering_system.h"
#include "../systems/animation_system.h"
#include "../systems/movement_system.h"
#include "../systems/input_movement_system.h"
#include "../systems/level_system.h"
#include "../systems/movement_animation_sync_system.h"
#include "../systems/behaviour_system.h"
#include "../systems/dialog_system.h"
#include "../systems/path_follow_system.h"

#include "../components/sprite_component.h"
#include "../components/impulse_component.h"
#include "../components/animation_component.h"
#include "../components/position_component.h"
#include "../components/camera_component.h"
#include "../components/input_movement_component.h"
#include "../components/dialog_component.h"

#include "../resources/animation.h"
#include "../resources/dialog.h"

#include "../utils/parser/ase_animation_parser.h"
#include "../map/map_loader.h"

#include "../levels/example/example_level.h"
#include "../levels/bsu_entrance/bsu_entrance_level.h"

PrototypeWidget::PrototypeWidget() {
  std::vector<std::unique_ptr<System>> systems;
  systems.emplace_back(std::make_unique<game::LevelSystem>(&level_context_));
  systems.emplace_back(
      std::make_unique<game::RenderingSystem>(&painter_context_,
                                              &window_context_));
  systems.emplace_back(std::make_unique<game::AnimationSystem>());
  systems.emplace_back(std::make_unique<game::InputMovementSystem>(&input_context_));
  systems.emplace_back(std::make_unique<game::MovementAnimationSyncSystem>());
  systems.emplace_back(std::make_unique<game::MovementSystem>());
  systems.emplace_back(std::make_unique<game::BehaviourSystem>());
  systems.emplace_back(std::make_unique<game::DialogSystem>(&input_context_,
                                                            &dialog_context_));
  systems.emplace_back(std::make_unique<game::PathFollowSystem>());

  world_.Init(std::move(systems));

  // init ui
  dialog_box_.setParent(this);
  dialog_context_.Init(&dialog_box_);

  // Load example level
  level_context_.Load<game::BsuEntranceLevel>();

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

void PrototypeWidget::keyReleaseEvent(QKeyEvent* event) {
  input_context_.RemoveKey(static_cast<Qt::Key>(event->key()));
}

void PrototypeWidget::resizeEvent(QResizeEvent* event) {
  dialog_box_.resize(width(), 100);
  dialog_box_.move(0, height() - 100);
}

