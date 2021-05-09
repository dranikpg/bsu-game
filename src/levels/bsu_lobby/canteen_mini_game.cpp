#include "canteen_mini_game.h"
#include <QResizeEvent>
#include <QPainter>
#include <utility>
#include <QDebug>
#include <random>

int game::CanteenMiniGame::timer = 0;
int game::CanteenMiniGame::curr_level = 1;
int game::CanteenMiniGame::lifes = 3;
game::CanteenMiniGame::State game::CanteenMiniGame::state = State::kWaiting;

void game::CanteenMiniGame::Process() {
  timer++;
  if (curr_num_chia_ == 0 && state == State::kWaiting) {
    if (timer > 270) {
      state = State::kGame;
      timer = 0;
      count_chia_ = 0;
      curr_num_chia_ = 0;
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
    if (curr_num_chia_ == 0) {
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
    if (state == State::kGame) {
      timer = 0;
      state = State::kGameOver;
    }
    if (timer > 200) {
      callback_();
      nimbus_->close();
    }
  } else if (curr_level > 3) {
    if (state != State::kGameEnding) {
      timer = 0;
      state = State::kGameEnding;
    }
    if (timer > 200) {
      callback_();
      nimbus_->close();
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
  nimbus_ = new CanteenDrawer(container, chiabatas_);
  chel_ty = std::make_shared<PlayerColumn>(PlayerColumn::kFist);
  nimbus_->player_state_ = chel_ty;
  nimbus_->setParent(container);
  nimbus_->resize(container->size());
  nimbus_->show();
  curr_speed_ = kFirstSpeed;
}

double game::CanteenMiniGame::CanteenDrawer::GetCoordinate(
                                  const game::CanteenMiniGame::PlayerColumn& state) {
  switch (state) {
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
  (*chiabatas_)[static_cast<PlayerColumn>(std::rand() % 4)].push_back(nimbus_->y() / 6.);
  count_chia_++;
  curr_num_chia_++;
}

void game::CanteenMiniGame::EraseChiabattas(const std::set<std::pair<PlayerColumn,
                                                                     double>>& for_erasing) {
  for (auto& chiabata : for_erasing) {
    auto elem = std::find((*chiabatas_)[chiabata.first].begin(),
                          (*chiabatas_)[chiabata.first].end(), chiabata.second);
    if (elem != (*chiabatas_)[chiabata.first].end()) {
      (*chiabatas_)[chiabata.first].erase(elem);
      curr_num_chia_--;
    }
  }
}

void game::CanteenMiniGame::ProccesChiabattas(
                            std::set<std::pair<PlayerColumn, double>>* for_erasing) {
  QPixmap chel(":/chel_for_bufet.png");
  for (auto& column : *chiabatas_) {
    for (auto& chiabata : column.second) {
      if (chiabata > container_->y() + container_->height() - chel.height()) {
        for_erasing->insert({column.first, chiabata});
        continue;
      }
      chiabata += 70;
      if (chiabata > nimbus_->y() + nimbus_->height() - chel.height()) {
        for_erasing->insert({column.first, chiabata});
        lifes--;
      }
    }
  }
}

void game::CanteenMiniGame::CollidingChiabattas(
                                    std::set<std::pair<PlayerColumn, double>>* for_erasing) {
  QPixmap chel(":/chel_for_bufet.png");
  QRect chelic(0,
               container_->y() + container_->height() - chel.height(),
               chel.width(),
               chel.height());
  QPixmap chia(":/chiabatta.png");
  for (auto chiabatta : (*chiabatas_)[*chel_ty]) {
    QRect kekes(0, chiabatta, chia.width(), chia.height());
    if (chelic.intersects(kekes)) {
      for_erasing->insert({*chel_ty, chiabatta});
    }
  }
}

void game::CanteenMiniGame::CanteenDrawer::paintEvent(QPaintEvent* event) {
  QPixmap background(":/canteen.png");
  QPainter painter(this);
  painter.save();
  painter.restore();
  painter.drawPixmap(container_->x(), container_->y(), container_->width(),
                     container_->height(), background);
  if (state == State::kWaiting && timer < 200) {
    QPixmap level;
    if (curr_level == 1) {
      level.load(":/bufet_levels1.png");
    } else if (curr_level == 2) {
      level.load(":/bufet_levels2.png");
    } else {
      level.load(":/bufet_levels3.png");
    }
    painter.drawPixmap(container_->x() + container_->width() / 2 - level.width() / 2,
                       container_->y() + container_->height() / 2 - level.height() / 2,
                       level);
  } else if (state == State::kGameEnding) {
    QPixmap game_end(":/bufet_levels5.png");
    painter.drawPixmap(container_->x() + container_->width() / 2 - game_end.width() / 2,
                       container_->y() + container_->height() / 2 - game_end.height() / 2,
                       game_end);
  } else if (state == State::kGameOver) {
    qDebug() << "aaa";
    QPixmap game_over(":/bufet_levels4.png");
    painter.drawPixmap(container_->x() + container_->width() / 2 - game_over.width() / 2,
                       container_->y() + container_->height() / 2 - game_over.height() / 2,
                       game_over);
  }
  QPixmap chiabata(":/chiabatta.png");
  for (const auto& column : *opa_) {
    for (auto coord : column.second) {
      painter.drawPixmap(
          QPointF(GetCoordinate(column.first) - chiabata.width() / 2., coord), chiabata);
    }
  }
  QPixmap chel(":/chel_for_bufet");
  painter.drawPixmap(QPointF(GetCoordinate(*player_state_) - chel.width() / 2.,
                             y() + height() - chel.height()), chel);
  QPixmap hearts;
  if (lifes == 3) {
    hearts.load(":/hearts1.png");
  } else if (lifes == 2) {
    hearts.load(":/hearts2.png");
  } else if (lifes == 1) {
    hearts.load(":/hearts3.png");
  }
  painter.drawPixmap(x(), y(), hearts);
}

void game::CanteenMiniGame::CanteenDrawer::resizeEvent(QResizeEvent* event) {
  resize(container_->size());
  update();
}

game::CanteenMiniGame::CanteenDrawer::CanteenDrawer(QWidget* container,
                                                    std::shared_ptr<std::map<PlayerColumn,
                                                  std::vector<double>>> opa)
                                                  : container_(container), opa_(std::move(opa)) {}
