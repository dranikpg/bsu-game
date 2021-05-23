#include "secret_mini_game.h"

#include <utility>
#include <QPainter>
#include <QPixmap>
#include <cmath>
#include <QDebug>

#include "../../components/animation_component.h"
#include "../../components/position_component.h"
#include "../../components/sprite_component.h"
#include "../../components/impulse_component.h"

namespace game {

SecretMiniGame::SecretMiniGame(SecretMiniGame::Callback callback, QWidget* container,
                               context::InputContext* input, ecs::World* world) :
                callback_(std::move(callback)), input_(input), kWaitingTime_(250),
                container_(container) {
  // creating drawer
  drawer_ = new Drawer(this);
  drawer_->setParent(container);
  drawer_->resize(container->size());
  drawer_->show();

  // creating "entities"
  player_ = new Player(world);
  target_ = new Target(world);
  lab_ = new Lab();
  player_->coordinate = QPointF(container->x() + container->width() / 2. -
                                  drawer_->player_pixmap_.width() / 8.,
                                container->y() + container->height() -
                                  drawer_->player_pixmap_.height());
  lab_->coordinate = for_updating_ - QPointF(drawer_->lab_pixmap_.width() / 2., 0);

  // general
  for_updating_ = QPointF(container->x() + container->width() / 2.,
                          container->y() + container->height()
                          - drawer_->player_pixmap_.height());
}

void SecretMiniGame::Process() {
  input_->BlockInput();
  if (player_->state != GameState::kGoodEnding &&
      player_->state != GameState::kBadEnding) {
    if (player_->state == GameState::kWaiting && clicked_) {
      // setting vector of lab
      double divisor = std::hypotf(drawer_->click_pos.x() - for_updating_.x(),
                                   drawer_->click_pos.y() - for_updating_.y());
      lab_->curr_vector = QPointF(
          (drawer_->click_pos.x() - for_updating_.x()) / divisor,
          (drawer_->click_pos.y() - for_updating_.y()) / divisor);
      player_->state = GameState::kThrowing;
      lab_->coordinate = for_updating_ - QPointF(drawer_->lab_pixmap_.width() / 2., 0);
      clicked_ = false;
    } else if (player_->state == GameState::kThrowing) {
      // updating lab and target
      // check if player catches target
      if (std::hypotf(lab_->coordinate.x() -
                      (target_->coordinate.x() - drawer_->target_pixmap_.width() / 18.),
                      lab_->coordinate.y() - target_->coordinate.y()) < 100 &&
                      !target_->is_hit) {
        target_->is_hit = true;
        player_->state = GameState::kReturningGood;
      } else if (lab_->coordinate.y() < container_->y() + container_->height() / 8. ||
                 lab_->coordinate.x() < container_->x() ||
                 lab_->coordinate.x() > container_->x() + container_->width()) {
        if (!target_->is_hit) {
          player_->state = GameState::kReturningBad;
        }
      } else {
        lab_->coordinate += lab_->kSpeed * lab_->curr_vector;
      }
    } else if (player_->state == GameState::kReturningGood ||
               player_->state == GameState::kReturningBad) {
      if (std::hypotf(player_->coordinate.x() - lab_->coordinate.x(),
                      player_->coordinate.y() - lab_->coordinate.y()) < 10) {
        if (player_->state == GameState::kReturningGood) {
          target_->is_running = false;
          player_->hits++;
        } else {
          player_->lives--;
        }
        player_->state = GameState::kWaiting;
      } else {
        lab_->coordinate -= lab_->kSpeed * lab_->curr_vector;
      }
    }
    // updating target
    if (!target_->is_running) {
      target_->side = (std::rand() % 2 == 0) ? 1 : -1;
      int screen_side = (target_->side < 0) ? container_->x() + container_->width() : container_->x();
      target_->coordinate = QPointF(screen_side, container_->y() + container_->height() / 8.);
      target_->is_hit = false;
      target_->speed = 6;
      target_->is_running = true;
      drawer_->changed_side_ = true;
    } else {
      if ((target_->side > 0 && target_->coordinate.x() > container_->x() + container_->width()) ||
          (target_->side < 0 && target_->coordinate.x() < container_->x())) {
        target_->is_running = false;
      } else if (player_->state != GameState::kReturningGood) {
        target_->coordinate += QPointF(target_->side * target_->speed, 0);
      }
    }
  }

  if (player_->lives == 0 && player_->state != GameState::kBadEnding) {
    player_->state = GameState::kBadEnding;
    kWaitingTime_ = 280;
  } else if (player_->hits == 1 && player_->state != GameState::kGoodEnding) {
    player_->state = GameState::kGoodEnding;
    kWaitingTime_ = 200;
  } else if (player_->state == GameState::kGoodEnding || player_->state == GameState::kBadEnding) {
    timer_++;
    if (timer_ > kWaitingTime_) {
      drawer_->media_player_->stop();
      drawer_->close();
      callback_();
    }
  }
}

SecretMiniGame::Drawer::Drawer(SecretMiniGame* game) : mini_game(game),
                                                       media_player_(new QMediaPlayer(this)),
                                                       first_playlist_(new QMediaPlaylist(this)),
                                                       second_playlist_(new QMediaPlaylist(this)),
                                                       third_playlist_(new QMediaPlaylist(this)){
  first_playlist_->addMedia(QUrl("qrc:/ulitka.mp3"));
  first_playlist_->setPlaybackMode(QMediaPlaylist::Loop);
  second_playlist_->addMedia(QUrl("qrc:/win.wav"));
  third_playlist_->addMedia(QUrl("qrc:/fail.wav"));
}

void SecretMiniGame::Drawer::resizeEvent(QResizeEvent* event) {
  QWidget::resizeEvent(event);
}

void SecretMiniGame::Drawer::paintEvent(QPaintEvent* event) {
  if (!started_) {
    media_player_->setPlaylist(first_playlist_);
    media_player_->play();
    started_ = true;
  }
  QPainter painter(this);
  painter.save();
  painter.restore();

  painter.drawPixmap(mini_game->container_->x(), mini_game->container_->y(),
                     mini_game->container_->width(), mini_game->container_->height(),
                     background_pixmap_);

  auto[animation] = mini_game->player_->player->Unpack<AnimationComponent>();
  QRect anim_rect = animation.animation_resource->GetFrame(animation.frame_index);
  painter.drawPixmap(mini_game->player_->coordinate, player_pixmap_, anim_rect);

  if (mini_game->player_->state == GameState::kThrowing) {
    painter.drawPixmap(mini_game->lab_->coordinate.x(), mini_game->lab_->coordinate.y(),
                       lab_pixmap_.width(), lab_pixmap_.height(),
                       lab_pixmap_);
  } else if (mini_game->player_->state == GameState::kReturningGood) {
    painter.drawPixmap(mini_game->lab_->coordinate.x() - ok_pixmap_.width() / 2. +
                           lab_pixmap_.width() / 2.,
                       mini_game->lab_->coordinate.y(), ok_pixmap_.width(),
                       ok_pixmap_.height(), ok_pixmap_);
  } else if (mini_game->player_->state == GameState::kReturningBad) {
    painter.drawPixmap(mini_game->lab_->coordinate.x() - wa_pixmap_.width() / 2. +
                           lab_pixmap_.width() / 2.,
                       mini_game->lab_->coordinate.y(),
                       wa_pixmap_.width(),
                       wa_pixmap_.height(),
                       wa_pixmap_);
  } else if (mini_game->player_->state == GameState::kBadEnding) {
    painter.drawPixmap(mini_game->container_->x() + mini_game->container_->width() / 2. -
                        bad_.width() / 2.,
                       mini_game->container_->y() + mini_game->container_->height() / 2. -
                       bad_.height() / 2.,
                       bad_);
  } else if (mini_game->player_->state == GameState::kGoodEnding) {
    painter.drawPixmap(mini_game->container_->x() + mini_game->container_->width() / 2. -
                       good_.width() / 2.,
                       mini_game->container_->y() + mini_game->container_->height() / 2. -
                       good_.width() / 2.,
                       good_);
  }

  if (mini_game->player_->lives > 0) {
    painter.drawPixmap(mini_game->container_->x(),
                       mini_game->container_->y() + mini_game->container_->height() -
                           hearts_pixmap_[mini_game->player_->lives - 1].height(),
                       hearts_pixmap_[mini_game->player_->lives - 1].width(),
                       hearts_pixmap_[mini_game->player_->lives - 1].height(),
                       hearts_pixmap_[mini_game->player_->lives - 1]);
  }
  if (!changed_playlist_) {
    if (mini_game->player_->state == GameState::kBadEnding) {
      media_player_->stop();
      media_player_->setPlaylist(third_playlist_);
      changed_playlist_ = true;
    } else if (mini_game->player_->state == GameState::kGoodEnding) {
      media_player_->stop();
      media_player_->setPlaylist(second_playlist_);
      changed_playlist_ = true;
    }
    media_player_->play();
  }

  if (mini_game->target_->is_running
      && mini_game->player_->state != GameState::kGoodEnding
      && mini_game->player_->state != GameState::kBadEnding) {
    auto [animation1] = mini_game->target_->target->Unpack<AnimationComponent>();
    if (mini_game->target_->is_hit && !stop_) {
      changed_side_ = true;
      if (count_ == 0) {
        animation1.SetAnimationResource(mini_game->target_->anims_["blowing"]);
      }
      QRect curr = animation1.animation_resource->GetFrame(animation1.frame_index);
      painter.drawPixmap(mini_game->target_->coordinate, target_pixmap_, curr);
      count_++;
      if (count_ > 75) {
        stop_ = true;
        count_ = 0;
      }
    } else if (!mini_game->target_->is_hit) {
      stop_ = false;
      if (mini_game->target_->side > 0 && changed_side_) {
        animation1.SetAnimationResource(mini_game->target_->anims_["running_right"]);
        changed_side_ = false;
      } else if (mini_game->target_->side < 0 && changed_side_) {
        animation1.SetAnimationResource(mini_game->target_->anims_["running_left"]);
        changed_side_ = false;
      }
      QRect curr = animation1.animation_resource->GetFrame(animation1.frame_index);
      painter.drawPixmap(mini_game->target_->coordinate, target_pixmap_, curr);
    }
  }
}

void SecretMiniGame::Drawer::mousePressEvent(QMouseEvent* event) {
  if (mini_game->player_->state == GameState::kWaiting) {
    click_pos = event->windowPos();
    mini_game->clicked_ = true;
  }
}

SecretMiniGame::Player::Player(ecs::World* world) {
  auto animations = utils::AseAnimationParser::Parse(QFile(":/chel_with_labs.json"));
  player = &world->CreateEntity()
      .AddComponent<AnimationComponent>(animations["coding_animation"])
      .AddComponent<SpriteComponent>(QRect(1, 1, 1, 1));
}

SecretMiniGame::Target::Target(ecs::World* world) {
  anims_ = utils::AseAnimationParser::Parse(QFile(":/target.json"));
  target = &world->CreateEntity().AddComponent<ImpulseComponent>()
      .AddComponent<AnimationComponent>(anims_["running_right"])
      .AddComponent<SpriteComponent>(QRect(1, 1, 1, 1));
}

SecretMiniGame::Lab::Lab() : kSpeed(7) {}

}  // namespace game
