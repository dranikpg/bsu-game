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

SecretMiniGame::GameState SecretMiniGame::player_state = GameState::kWaiting;
QPointF SecretMiniGame::lab_coordinates = QPointF(0, 0);
QPointF SecretMiniGame::player = QPointF(0, 0);
QPointF SecretMiniGame::curr_vector = QPointF(0, 0);
QPointF SecretMiniGame::Drawer::click_pos = QPointF(0, 0);
bool SecretMiniGame::clicked = false;
std::vector<std::pair<QPointF, bool>> SecretMiniGame::coordinates = {};
int SecretMiniGame::lives = 3;

SecretMiniGame::SecretMiniGame(SecretMiniGame::Callback callback, QWidget* container,
                               context::InputContext* input, ecs::World* world) :
                                callback_(std::move(callback)), container_(container),
                                input_(input), world_(world),
                                kLabSpeed_(7), kTargetSpeed_(6), kWaitingTime_(200) {
  drawer_ = new Drawer(container, world_);
  drawer_->setParent(container);
  drawer_->resize(container->size());
  drawer_->show();
  player = QPointF(container_->x() + container_->width() / 2. -
                    drawer_->player_pixmap_.width() / 8.,
                   container_->y() + container_->height() -
                  drawer_->player_pixmap_.height());
  for_updating_ = QPointF(container_->x() + container_->width() / 2.,
                          container_->y() + container_->height()
                          - drawer_->player_pixmap_.height());
  lab_coordinates = for_updating_ - QPointF(drawer_->lab_pixmap_.width() / 2., 0);
  player_state = GameState::kWaiting;
}

void SecretMiniGame::Process() {
  input_->BlockInput();
  if (player_state != GameState::kGoodEnding && player_state != GameState::kBadEnding) {
    if (player_state == GameState::kWaiting && clicked) {
      double divisor = std::hypotf(SecretMiniGame::Drawer::click_pos.x() - for_updating_.x(),
                                   SecretMiniGame::Drawer::click_pos.y() - for_updating_.y());
      curr_vector = QPointF(
          (SecretMiniGame::Drawer::click_pos.x() - for_updating_.x()) / divisor,
          (SecretMiniGame::Drawer::click_pos.y() - for_updating_.y()) / divisor);
      player_state = GameState::kThrowing;
      lab_coordinates = for_updating_ - QPointF(drawer_->lab_pixmap_.width() / 2., 0);
      clicked = false;
    } else if (player_state == GameState::kThrowing) {
      for (auto& coordinate : coordinates) {
        if (std::hypotf(lab_coordinates.x() -
                        (coordinate.first.x() - drawer_->target_pixmap_.width() / 18.),
                        lab_coordinates.y() - coordinate.first.y()) < 100 && !coordinate.second) {
          coordinate.second = true;
          player_state = GameState::kReturningGood;
        }
      }
      if (lab_coordinates.y() < container_->y() + container_->height() / 8. ||
          lab_coordinates.x() < container_->x() ||
          lab_coordinates.x() > container_->x() + container_->width()) {
        if (coordinates[0].second) {
          player_state = GameState::kReturningGood;
        } else {
          player_state = GameState::kReturningBad;
        }
      } else {
        lab_coordinates += kLabSpeed_ * curr_vector;
      }
    } else if (player_state == GameState::kReturningGood ||
        player_state == GameState::kReturningBad) {
      if (std::hypotf(player.x() - lab_coordinates.x(), player.y() - lab_coordinates.y()) < 10) {
        if (player_state == GameState::kReturningGood) {
          coordinates.resize(0);
          hits_++;
        } else {
          lives--;
        }
        player_state = GameState::kWaiting;
      } else {
        lab_coordinates -= kLabSpeed_ * curr_vector;
      }
    }

    if (coordinates.empty()) {
      coordinates.emplace_back(
          QPointF(container_->x(), container_->y() + container_->height() / 8.),
          false);
    } else {
      if (coordinates[0].first.x() > container_->x() + container_->width()) {
        coordinates.resize(0);
      } else if (player_state != GameState::kReturningGood) {
        coordinates[0].first += QPointF(kTargetSpeed_, 0);
      }
    }
  }

  if (lives == 0 && player_state != GameState::kBadEnding) {
    player_state = GameState::kBadEnding;
  } else if (hits_ == 5 && player_state != GameState::kGoodEnding) {
    player_state = GameState::kGoodEnding;
  } else if (player_state == GameState::kGoodEnding || player_state == GameState::kBadEnding) {
    timer_++;
    if (timer_ > kWaitingTime_) {
      callback_();
      drawer_->close();
    }
  }
}

SecretMiniGame::Drawer::Drawer(QWidget* container, ecs::World* world) : container_(container) {
  auto animations = utils::AseAnimationParser::Parse(QFile(":/chel_with_labs.json"));
  player_ = &world->CreateEntity()
      .AddComponent<AnimationComponent>(animations["coding_animation"])
      .AddComponent<SpriteComponent>(QRect(1, 1, 1, 1));

  anims_ = utils::AseAnimationParser::Parse(QFile(":/target.json"));
  target_ = &world->CreateEntity().AddComponent<ImpulseComponent>()
      .AddComponent<AnimationComponent>(anims_["running_right"])
      .AddComponent<SpriteComponent>(QRect(1, 1, 1, 1));
}

void SecretMiniGame::Drawer::resizeEvent(QResizeEvent* event) {
  QWidget::resizeEvent(event);
}

void SecretMiniGame::Drawer::paintEvent(QPaintEvent* event) {
  QPainter painter(this);
  painter.save();
  painter.restore();

  painter.drawPixmap(container_->x(), container_->y(), container_->width(),
                     container_->height(), background_pixmap_);

  auto [animation] = player_->Unpack<AnimationComponent>();
  QRect anim_rect = animation.animation_resource->GetFrame(animation.frame_index);
  painter.drawPixmap(player, player_pixmap_, anim_rect);

  if (player_state == GameState::kThrowing) {
    painter.drawPixmap(lab_coordinates.x(),
                       lab_coordinates.y(),
                       lab_pixmap_.width(),
                       lab_pixmap_.height(),
                       lab_pixmap_);
  } else if (player_state == GameState::kReturningGood) {
  painter.drawPixmap(lab_coordinates.x() - ok_pixmap_.width() / 2. + lab_pixmap_.width() / 2.,
                     lab_coordinates.y(),
                     ok_pixmap_.width(),
                     ok_pixmap_.height(),
                     ok_pixmap_);
  } else if (player_state == GameState::kReturningBad) {
      painter.drawPixmap(lab_coordinates.x() - wa_pixmap_.width() / 2. + lab_pixmap_.width() / 2.,
                       lab_coordinates.y(),
                       wa_pixmap_.width(),
                       wa_pixmap_.height(),
                       wa_pixmap_);
  }
  if (lives > 0) {
    painter.drawPixmap(container_->x(),
                       container_->y() + container_->height() - hearts_pixmap_[lives - 1].height(),
                       hearts_pixmap_[lives - 1].width(),
                       hearts_pixmap_[lives - 1].height(),
                       hearts_pixmap_[lives - 1]);
  }

  if (!coordinates.empty()
      && player_state != GameState::kGoodEnding
      && player_state != GameState::kBadEnding) {
    auto [animation1] = target_->Unpack<AnimationComponent>();
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
    painter.drawPixmap(coordinates[0].first, target_pixmap_, curr);
  }
}

void SecretMiniGame::Drawer::mousePressEvent(QMouseEvent* event) {
  if (player_state == GameState::kWaiting) {
    click_pos = event->windowPos();
    clicked = true;
  }
}

}  // namespace game
