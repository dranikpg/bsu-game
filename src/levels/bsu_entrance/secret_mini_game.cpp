#include "secret_mini_game.h"

#include <utility>
#include <QPainter>
#include <QPixmap>
#include <cmath>

#include "../../components/animation_component.h"
#include "../../components/position_component.h"
#include "../../components/sprite_component.h"
#include "../../components/impulse_component.h"

namespace game {

SecretMiniGame::PudgeState SecretMiniGame::pudge_state = PudgeState::kWaiting;
QPointF SecretMiniGame::hook_coordinates = QPointF(0, 0);
QPointF SecretMiniGame::pudge = QPointF(0, 0);
QPointF SecretMiniGame::curr_vector = QPointF(0, 0);
QPointF SecretMiniGame::Drawer::click_pos = QPointF(0, 0);
bool SecretMiniGame::clicked = false;
std::vector<std::pair<QPointF, bool>> SecretMiniGame::coordinates = {};
int SecretMiniGame::lifes = 3;

SecretMiniGame::SecretMiniGame(SecretMiniGame::Callback callback,
                               QWidget* container,
                               context::InputContext* input,
                               ecs::World* world) :
                                callback_(std::move(callback)),
                                container_(container),
                                input_(input),
                                world_(world) {
  drawer_ = new Drawer(container, world_);
  drawer_->setParent(container);
  drawer_->resize(container->size());
  drawer_->show();
  pudge = QPointF(container_->x() + container_->width() / 2. -
                  QPixmap(":/chel_with_labs.png").width() / 8.,
                  container_->y() + container_->height() -
                  QPixmap(":/chel_with_labs.png").height());
  for_updating_ = QPointF(container_->x() + container_->width() / 2.,
                          container_->y() + container_->height()
                          - QPixmap(":/chel_with_labs.png").height());
  hook_coordinates = for_updating_ - QPointF(QPixmap(":/laba.png").width() / 2.,
                                             0);
  pudge_state = PudgeState::kWaiting;
}

void SecretMiniGame::Process() {
  input_->BlockInput();
  if (pudge_state != PudgeState::kGoodEnding && pudge_state != PudgeState::kBadEnding) {
    if (pudge_state == PudgeState::kWaiting && clicked) {
      curr_vector = QPointF(
          (SecretMiniGame::Drawer::click_pos.x() - for_updating_.x()) /
              std::hypotf(SecretMiniGame::Drawer::click_pos.x() - for_updating_.x(),
                          SecretMiniGame::Drawer::click_pos.y() - for_updating_.y()),
          (SecretMiniGame::Drawer::click_pos.y() - for_updating_.y()) /
              std::hypotf(SecretMiniGame::Drawer::click_pos.x() - for_updating_.x(),
                          SecretMiniGame::Drawer::click_pos.y() - for_updating_.y()));
      pudge_state = PudgeState::kHooking;
      hook_coordinates = for_updating_ - QPointF(QPixmap(":/laba.png").width() / 2.,
                                                 0);
      clicked = false;
    } else if (pudge_state == PudgeState::kHooking) {
      for (auto& coordinate : coordinates) {
        if (std::hypotf(hook_coordinates.x() -
                            (coordinate.first.x() - QPixmap(":/target.png").width() / 18.),
                        hook_coordinates.y() - coordinate.first.y()) < 100 && !coordinate.second) {
          coordinate.second = true;
          pudge_state = PudgeState::kReturningGood;
        }
      }
      if (hook_coordinates.y() < container_->y() + container_->height() / 8. ||
          hook_coordinates.x() < container_->x() ||
          hook_coordinates.x() > container_->x() + container_->width()) {
        if (coordinates[0].second) {
          pudge_state = PudgeState::kReturningGood;
        } else {
          pudge_state = PudgeState::kReturningBad;
        }
      } else {
        hook_coordinates += 7 * curr_vector;
      }
    } else if (pudge_state == PudgeState::kReturningGood ||
        pudge_state == PudgeState::kReturningBad) {
      if (std::hypotf(pudge.x() - hook_coordinates.x(), pudge.y() - hook_coordinates.y()) < 10) {
        if (pudge_state == PudgeState::kReturningGood) {
          coordinates.resize(0);
          hits_++;
        } else {
          lifes--;
        }
        pudge_state = PudgeState::kWaiting;
      } else {
        hook_coordinates -= 7 * curr_vector;
      }
    }

    if (coordinates.empty()) {
      coordinates.emplace_back(
          QPointF(container_->x(), container_->y() + container_->height() / 8.),
          false);
    } else {
      if (coordinates[0].first.x() > container_->x() + container_->width()) {
        coordinates.resize(0);
      } else if (pudge_state != PudgeState::kReturningGood) {
        coordinates[0].first += QPointF(6, 0);
      }
    }
  }

  if (lifes == 0 && pudge_state != PudgeState::kBadEnding) {
    pudge_state = PudgeState::kBadEnding;
  } else if (hits_ == 5 && pudge_state != PudgeState::kGoodEnding) {
    pudge_state = PudgeState::kGoodEnding;
  } else if (pudge_state == PudgeState::kGoodEnding || pudge_state == PudgeState::kBadEnding) {
    timer_++;
    if (timer_ > 200) {
      callback_();
      drawer_->close();
    }
  }
}

SecretMiniGame::Drawer::Drawer(QWidget* container, ecs::World* world) : container_(container) {
  auto animations = utils::AseAnimationParser::Parse(QFile(":/chel_with_labs.json"));
  pudge_ = &world->CreateEntity()
      .AddComponent<AnimationComponent>(animations["coding_animation"])
      .AddComponent<SpriteComponent>(QRect(1, 1, 1, 1));

  anims_ = utils::AseAnimationParser::Parse(QFile(":/target.json"));
  target = &world->CreateEntity().AddComponent<ImpulseComponent>()
      .AddComponent<AnimationComponent>(anims_["running_right"])
      .AddComponent<SpriteComponent>(QRect(1,1,1,1));
}

void SecretMiniGame::Drawer::resizeEvent(QResizeEvent* event) {
  QWidget::resizeEvent(event);
}

void SecretMiniGame::Drawer::paintEvent(QPaintEvent* event) {
  QPainter painter(this);
  painter.save();
  painter.restore();

  painter.drawPixmap(container_->x(), container_->y(), container_->width(),
                     container_->height(), background_);

  // drawing pudge
  auto [animation] = pudge_->Unpack<AnimationComponent>();
  QRect opa = animation.animation_resource->GetFrame(animation.frame_index);
  painter.drawPixmap(pudge, pudge_pixmap_, opa);

  if (pudge_state == PudgeState::kHooking) {
    painter.drawPixmap(hook_coordinates.x(),
                       hook_coordinates.y(),
                       hook.width(),
                       hook.height(),
                       hook);
  } else if (pudge_state == PudgeState::kReturningGood) {
  painter.drawPixmap(hook_coordinates.x() - ok_.width() / 2. + hook.width() / 2.,
                     hook_coordinates.y(),
                     ok_.width(),
                     ok_.height(),
                     ok_);
  } else if (pudge_state == PudgeState::kReturningBad) {
    painter.drawPixmap(hook_coordinates.x() - wa_.width() / 2. + hook.width() / 2.,
                       hook_coordinates.y(),
                       wa_.width(),
                       wa_.height(),
                       wa_);
  }
  if (lifes > 0) {
    painter.drawPixmap(container_->x(),
                       container_->y() + container_->height() - hearts_[lifes - 1].height(),
                       hearts_[lifes - 1].width(),
                       hearts_[lifes - 1].height(),
                       hearts_[lifes - 1]);
  }

  if (!coordinates.empty()
      && pudge_state != PudgeState::kGoodEnding
      && pudge_state != PudgeState::kBadEnding) {
    auto [animation1] = target->Unpack<AnimationComponent>();
    if (coordinates[0].second && !stop_) {
      if (!is_changed) {
        animation1.SetAnimationResource(anims_["blowing"]);
        is_changed = true;
        count_ = 0;
      }
      if (animation1.frame_index == 4 && count_ >= 75) {
        stop_ = true;
      }
    } else {
      if (is_changed) {
        animation1.SetAnimationResource(anims_["running_right"]);
        is_changed = false;
        stop_ = false;
      }
    }
    QRect curr = animation1.animation_resource->GetFrame(animation1.frame_index);
    painter.drawPixmap(coordinates[0].first, QPixmap(":/target.png"), curr);
  }
}

void SecretMiniGame::Drawer::mousePressEvent(QMouseEvent* event) {
  if (pudge_state == PudgeState::kWaiting) {
    click_pos = event->windowPos();
    clicked = true;
  }
}

}  // namespace game
