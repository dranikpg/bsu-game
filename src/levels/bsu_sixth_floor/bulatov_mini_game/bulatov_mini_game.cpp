#include "bulatov_mini_game.h"

#include "../../../utils/parser/ase_animation_parser.h"
#include "../../../components/components.h"
#include "../../../utils/parser/dialog_parser.h"

#include <QPainter>
#include <QHBoxLayout>

#include <QDebug>

namespace game {

void BulatovMiniGame::Process() {
  drawer_->Process();
}

BulatovMiniGame::BulatovMiniGame(Callback callback, QWidget* container,
                                 ecs::World* world) {
  drawer_ = new Drawer(callback, container, world);
  drawer_->setParent(container);
  drawer_->resize(container->size());
  drawer_->show();
}

void BulatovMiniGame::Drawer::Process() {
  switch (game_state_) {
    case GameState::kStart: {
      chernov_player_ = &world_->CreateEntity()
          .AddComponent<SpriteComponent>(QRect(0,0,0,0))
          .AddComponent<AnimationComponent>(chernov_animation_["speak"]);
      game_state_ = GameState::kDialog;
    }
  }
}


BulatovMiniGame::Drawer::Drawer(Callback callback, QWidget* container,
                                ecs::World* world)
                                : callback_(std::move(callback)),
                                  container_(container),
                                  world_(world) {
  resize(container_->size());
  auto container_layout = new QHBoxLayout();
  container_layout->addWidget(this);
  container_layout->setSpacing(0);
  container_layout->setContentsMargins(0,0,0,0);
  container_->setLayout(container_layout);

  auto main_layout = new QHBoxLayout(this);
  main_layout->setSpacing(0);
  main_layout->setContentsMargins(0,0,0,0);
  setLayout(main_layout);

  background_ = QPixmap(":/chernov_challenge_background.png");
  chernov_animation_ = utils::AseAnimationParser::Parse(
      QFile(":/chernov.json"));
  chernov_ = QPixmap(":/chernov.png");

  player_dialog_container_ = new QWidget(this);
  chernov_dialog_container_ = new QWidget(this);

  RecalculateSizes();
}

void BulatovMiniGame::Drawer::RecalculateSizes() {
  chernov_screen_bounds_ = QRect{static_cast<int>(0.734*container_->width()),
                          static_cast<int>(0.498*container_->height()),
                          static_cast<int>(0.129*container_->width()),
                          static_cast<int>(0.214*container_->height())};
  chernov_dialog_container_->setGeometry(static_cast<int>(0.484*container_->width()),
                                   static_cast<int>(0.324*container_->height()),
                                   static_cast<int>(0.275*container_->width()),
                                   static_cast<int>(0.175*container_->height()));
  player_dialog_container_->setGeometry(static_cast<int>(0.301*container_->width()),
                                   static_cast<int>(0.59*container_->height()),
                                   static_cast<int>(0.247*container_->width()),
                                   static_cast<int>(0.166*container_->height()));
}

void BulatovMiniGame::Drawer::paintEvent(QPaintEvent* event) {
  QPainter painter(this);
  painter.drawPixmap(QRect{0,0,width(),height()}, background_,
                     QRect{0,0,background_.width(),background_.height()});
  ComputeChernovBounds();
  painter.drawPixmap(chernov_screen_bounds_, chernov_, chernov_pixmap_bounds_);
}

void BulatovMiniGame::Drawer::ComputeChernovBounds() {
  if (chernov_player_ != nullptr) {
    auto& component = chernov_player_->GetComponent<AnimationComponent>();
    auto frame_index = component.frame_index;
    chernov_pixmap_bounds_ = component.animation_resource->GetFrame(frame_index);
  }
}

void BulatovMiniGame::Drawer::resizeEvent(QResizeEvent* event) {
  resize(container_->size());
  RecalculateSizes();
  update();
}


}  // namespace game