#include "canteen_mini_game.h"
#include <QResizeEvent>
#include <QPainter>
#include <utility>
#include <random>
#include <QDebug>

int game::CanteenMiniGame::timer = 0;
int game::CanteenMiniGame::curr_level = 1;
int game::CanteenMiniGame::lifes = 3;
game::CanteenMiniGame::State game::CanteenMiniGame::state = State::kWaiting;

void game::CanteenMiniGame::Process() {
  timer++;
  if (current_chiabatta_ == 0 && state == State::kWaiting) {
    if (timer > 270) {
      state = State::kGame;
      timer = 0;
      count_chia_ = 0;
      current_chiabatta_ = 0;
      if (curr_level == 1) {
        curr_speed_ = kFirstSpeed;
      } else if (curr_level == 2) {
        curr_speed_ = kSecondSpeed;
      } else {
        curr_speed_ = kUnreal;
      }
    }
  }

  std::set<std::pair<PlayerColumn, double>> for_erase;
  if (timer % curr_speed_ == 0 && state == State::kGame) {
    CollidingChiabattas(&for_erase);
    EraseChiabattas(for_erase);
    for_erase.clear();
    if (count_chia_ <= 10) {
      AddChiabata(timer);
    }
    ProccesChiabattas(&for_erase);
    EraseChiabattas(for_erase);
    if (current_chiabatta_ == 0) {
      timer = 0;
      state = State::kWaiting;
      curr_level++;
    }
  }

  if (input_->GetFrameKeys().count(Keys::kLeft) > 0) {
    if (*chel_ty != PlayerColumn::kFist) {
      *chel_ty = static_cast<PlayerColumn>(static_cast<int>(*chel_ty) - 1);
    }
  } else if (input_->GetFrameKeys().count(Keys::kRight) > 0) {
    if (*chel_ty != PlayerColumn::kFourth) {
      *chel_ty = static_cast<PlayerColumn>(static_cast<int>(*chel_ty) + 1);
    }
  }
  input_->BlockInput();

  if (lifes == 0) {
    if (state != State::kGameOver) {
      timer = 0;
      state = State::kGameOver;
    }
    if (timer > 280) {
      state = State::kWaiting;
      timer = 0;
      curr_level = 1;
      lifes = 3;
      canteen_->close();
      callback_();
    }
  } else if (curr_level > 3) {
    if (state != State::kGameEnding) {
      timer = 0;
      state = State::kGameEnding;
    }
    if (timer > 200) {
      state = State::kWaiting;
      timer = 0;
      curr_level = 1;
      lifes = 3;
      canteen_->close();
      callback_();
    }
  }
}

game::CanteenMiniGame::CanteenMiniGame(game::CanteenMiniGame::Callback callback,
                                       QWidget* container,
                                       context::InputContext* input) :
                                       callback_(std::move(callback)),
                                       input_(input) {
  container_ = container;
  chiabatas_ = std::make_shared<std::map<PlayerColumn, std::vector<double>>>();
  canteen_ = new CanteenDrawer(container, chiabatas_);
  chel_ty = std::make_shared<PlayerColumn>(PlayerColumn::kFist);
  canteen_->player_state_ = chel_ty;
  canteen_->setParent(container);
  canteen_->resize(container->size());
  canteen_->show();
  curr_speed_ = kFirstSpeed;
}

double game::CanteenMiniGame::CanteenDrawer::GetCoordinate(
                                  const game::CanteenMiniGame::PlayerColumn& pl_state) {
  switch (pl_state) {
    case PlayerColumn::kFist:
      return container_->width() / 8.;
    case PlayerColumn::kSecond:
      return container_->width() / 4. + container_->width() / 8.;
    case PlayerColumn::kThird:
      return container_->width() / 2. + container_->width() / 8.;
    case PlayerColumn::kFourth:
      return 3. * container_->width() / 4. + container_->width() / 8.;
  }
}

void game::CanteenMiniGame::AddChiabata(int rand) {
  (*chiabatas_)[static_cast<PlayerColumn>(std::rand() % 4)].push_back(canteen_->y() / 6.);
  count_chia_++;
  current_chiabatta_++;
}

void game::CanteenMiniGame::EraseChiabattas(const std::set<std::pair<PlayerColumn,
                                                                     double>>& for_erasing) {
  for (auto& chiabata : for_erasing) {
    auto elem = std::find((*chiabatas_)[chiabata.first].begin(),
                          (*chiabatas_)[chiabata.first].end(), chiabata.second);
    if (elem != (*chiabatas_)[chiabata.first].end()) {
      (*chiabatas_)[chiabata.first].erase(elem);
      current_chiabatta_--;
    }
  }
}

void game::CanteenMiniGame::ProccesChiabattas(
                            std::set<std::pair<PlayerColumn, double>>* for_erasing) {
  for (auto& column : *chiabatas_) {
    for (auto& chiabata : column.second) {
      if (chiabata > container_->y() + container_->height() - chel_size_.height()) {
        for_erasing->insert({column.first, chiabata});
        continue;
      }
      chiabata += 100;
      if (chiabata > canteen_->y() + canteen_->height() - chel_size_.height()) {
        for_erasing->insert({column.first, chiabata});
        lifes--;
      }
    }
  }
}

void game::CanteenMiniGame::CollidingChiabattas(
                                    std::set<std::pair<PlayerColumn, double>>* for_erasing) {
  QRect chelic(0,
               container_->y() + container_->height() - chel_size_.height(),
               chel_size_.width(),
               chel_size_.height());
  for (auto chiabatta : (*chiabatas_)[*chel_ty]) {
    QRect kekes(0, chiabatta, chia_size_.width(), chia_size_.height());
    if (chelic.intersects(kekes)) {
      for_erasing->insert({*chel_ty, chiabatta});
    }
  }
}

void game::CanteenMiniGame::CanteenDrawer::paintEvent(QPaintEvent* event) {
  if (!started) {
    media_player_->setPlaylist(first_playlist_);
    media_player_->play();
    started = true;
  }
  QPainter painter(this);
  painter.save();
  painter.restore();
  painter.drawPixmap(container_->x(), container_->y(), container_->width(),
                     container_->height(), background_);
  if (state == State::kWaiting && timer < 200) {
    painter.drawPixmap(
        container_->x() + container_->width() / 2 - levels_[curr_level - 1].width() / 2,
        container_->y() + container_->height() / 2 - levels_[curr_level - 1].height() / 2,
        levels_[curr_level - 1]);
  } else if (state == State::kGameEnding) {
    if (!changed_playlist) {
      media_player_->stop();
      changed_playlist = true;
      media_player_->setPlaylist(second_playlist_);
      media_player_->play();
    }
    painter.drawPixmap(container_->x() + container_->width() / 2 - levels_[4].width() / 2,
                       container_->y() + container_->height() / 2 - levels_[4].height() / 2,
                       levels_[4]);
  } else if (state == State::kGameOver) {
    if (!changed_playlist) {
      media_player_->stop();
      changed_playlist = true;
      media_player_->setPlaylist(third_playlist_);
      media_player_->play();
    }
    painter.drawPixmap(container_->x() + container_->width() / 2 - levels_[3].width() / 2,
                       container_->y() + container_->height() / 2 - levels_[3].height() / 2,
                       levels_[3]);
  }
  if (state != State::kGameOver) {
    for (const auto& column : *opa_) {
      for (auto coord : column.second) {
        painter.drawPixmap(
            QPointF(GetCoordinate(column.first) - chiabata_.width() / 2., coord),
                        chiabata_);
      }
    }
  }
  painter.drawPixmap(QPointF(GetCoordinate(*player_state_) - chel_.width() / 2.,
                             y() + height() - chel_.height()), chel_);
  if (lifes > 0) {
    painter.drawPixmap(x(), y(), hearts_[lifes - 1]);
  }
}

void game::CanteenMiniGame::CanteenDrawer::resizeEvent(QResizeEvent* event) {
  resize(container_->size());
  update();
}

game::CanteenMiniGame::CanteenDrawer::CanteenDrawer(QWidget* container,
                                                    std::shared_ptr<std::map<PlayerColumn,
                                                  std::vector<double>>> opa)
                                                  : container_(container), opa_(std::move(opa)) {
  media_player_ = new QMediaPlayer(this);
  first_playlist_ = new QMediaPlaylist(this);
  second_playlist_ = new QMediaPlaylist(this);
  third_playlist_ = new QMediaPlaylist(this);
  first_playlist_->addMedia(QUrl("qrc:/ulitka.mp3"));
  first_playlist_->setPlaybackMode(QMediaPlaylist::Loop);
  second_playlist_->addMedia(QUrl("qrc:/win.wav"));
  third_playlist_->addMedia(QUrl("qrc:/fail.wav"));
}
