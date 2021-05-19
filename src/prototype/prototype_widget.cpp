#include "prototype_widget.h"

#include <vector>
#include <utility>
#include <memory>
#include <map>

#include <QPainter>
#include <QTimer>
#include <QPushButton>
#include <QFontDatabase>

#include "../systems/rendering_system.h"
#include "../systems/animation_system.h"
#include "../systems/movement_system.h"
#include "../systems/input_movement_system.h"
#include "../systems/level_system.h"
#include "../systems/movement_animation_sync_system.h"
#include "../systems/behaviour_system.h"
#include "../systems/dialog_system.h"
#include "../systems/path_follow_system.h"
#include "../systems/splash_system.h"
#include "../systems/camera_system.h"

#include "../components/components.h"

#include "../resources/animation.h"
#include "../resources/dialog.h"
#include "../utils/parser/ase_animation_parser.h"
#include "../map/map_loader.h"

#include "../levels/bsu_entrance/bsu_entrance_level.h"

PrototypeWidget::PrototypeWidget() {
  std::vector<std::unique_ptr<System>> systems;
  systems.emplace_back(
      std::make_unique<game::LevelSystem>(&level_context_, &mini_game_context_, &input_context_));
  systems.emplace_back(std::make_unique<game::SplashSystem>(&splash_context_, &input_context_));
  systems.emplace_back(
      std::make_unique<game::RenderingSystem>(&painter_context_, &window_context_));
  systems.emplace_back(std::make_unique<game::AnimationSystem>());
  systems.emplace_back(std::make_unique<game::InputMovementSystem>(&input_context_));
  systems.emplace_back(std::make_unique<game::MovementAnimationSyncSystem>());
  systems.emplace_back(std::make_unique<game::MovementSystem>());
  systems.emplace_back(std::make_unique<game::BehaviourSystem>());
  systems.emplace_back(std::make_unique<game::DialogSystem>(&input_context_, &dialog_context_));
  systems.emplace_back(std::make_unique<game::PathFollowSystem>());
  systems.emplace_back(std::make_unique<game::CameraSystem>(&window_context_));

  world_.Init(std::move(systems));

  // load font
  QFontDatabase::addApplicationFont(":/chrono_type.ttf");

  // init ui
  dialog_box_.setParent(this);
  dialog_context_.Init(&dialog_box_);
  mini_game_box_.setParent(this);
  mini_game_context_.Init(&mini_game_box_);
  splash_box_.setParent(this);
  splash_context_.Init(&splash_box_);

  // Load first level
  level_context_.Load<game::BsuEntranceLevel>();

  QPixmap icon(":/guard-sheet.png");
  world_.CreateEntity().AddComponent<game::SplashComponent>(
      utils::PixmapRect(icon, QRect(0, 0, 64, 64)),
      "Hello! I'm your master \nWelcome to the game!",
      []() {
      });

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
  mini_game_box_.resize(width(), height());
  splash_box_.resize(width(), height());
}

