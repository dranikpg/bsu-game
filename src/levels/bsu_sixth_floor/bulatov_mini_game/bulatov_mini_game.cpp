#include "bulatov_mini_game.h"

#include "../../../utils/parser/ase_animation_parser.h"
#include "../../../components/components.h"

#include <QPainter>

#include <QDebug>

namespace game {

void BulatovMiniGame::Process() {

}

BulatovMiniGame::BulatovMiniGame(Callback callback, QWidget* container,
                                 ecs::World* world)
  : callback_(std::move(callback)) {
  drawer_ = new Drawer(container, world);
  drawer_->setParent(container);
  drawer_->resize(container->size());
  drawer_->show();
}

void BulatovMiniGame::Drawer::LoadAnimations() {
  animations_ = utils::AseAnimationParser::Parse(
      QFile(":/bulatov_chellenge.json"));
  background_ = QPixmap(":/bulatov_chellenge.png");
}

void BulatovMiniGame::Drawer::LoadFrameBounds() {
  auto& component = animation_player->GetComponent<AnimationComponent>();
  auto frame_index =
      component.frame_index;
  current_frame_bounds_ =
      component.animation_resource->GetFrame(frame_index);
}

BulatovMiniGame::Drawer::Drawer(QWidget* container,
                                ecs::World* world)
                                : container_(container),
                                  world_(world) {
  LoadAnimations();
  animation_player = &world_->CreateEntity()
      .AddComponent<AnimationComponent>(animations_["no_1"])
      .AddComponent<SpriteComponent>(QRect(0,0,0,0));

  answer_widget_ = new AnswerWidget(this);

}

void BulatovMiniGame::Drawer::paintEvent(QPaintEvent* event) {
  QPainter painter(this);
  QRect target = {0, 0, container_->width(), container_->height()};
  LoadFrameBounds();
  painter.drawPixmap(target, background_, current_frame_bounds_);
}

void BulatovMiniGame::Drawer::resizeEvent(QResizeEvent* event) {
  resize(container_->size());
  update();
}

}  // namespace game