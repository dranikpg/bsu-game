#include "labyrinth_mini_game.h"

#include <QResizeEvent>
#include <QPainter>
#include <utility>
#include <random>
#include <QRegion>
#include <QRect>
#include <QDebug>
#include <QVBoxLayout>
#include <QRadialGradient>
#include <QBrush>

void game::LabyrinthMiniGame::Process(QPointF player_pos) {
  player_pos_ = player_pos;
}

game::LabyrinthMiniGame::LabyrinthMiniGame(game::LabyrinthMiniGame::Callback callback,
                                           QWidget* container,
                                           context::InputContext* input,
                                           std::shared_ptr<bool> is_switched) :
    callback_(std::move(callback)),
    input_(input),
    container_(container), is_sw_(std::move(is_switched)) {
  drawer_ = new Drawer(this);
  if (container->layout() != nullptr) {
    delete container->layout();
  }
  auto* layout = new QVBoxLayout(container);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(drawer_);
}

void game::LabyrinthMiniGame::Drawer::paintEvent(QPaintEvent* event) {
  QPainter painter(this);
  if (abs(mini_game_->player_pos_.x()) > 3000 || abs(mini_game_->player_pos_.y()) > 3000) {
    painter.fillRect(mini_game_->container_->rect(), Qt::black);
    return;
  }
  if (mini_game_->player_pos_.x() != mini_game_->player_pos_.x()) {
    painter.fillRect(mini_game_->container_->rect(), Qt::black);
    return;
  }

  if (!*(mini_game_->is_sw_)) {
    rect_radial = QRect(mini_game_->container_->rect());
    radialGrad.setFocalPoint(mini_game_->player_pos_);
    radialGrad.setRadius(mini_game_->container_->rect().height() / 2.);
    radialGrad.setCenter(mini_game_->player_pos_);
    painter.fillRect(rect_radial, radialGrad);
  }
}

void game::LabyrinthMiniGame::Drawer::resizeEvent(QResizeEvent* event) {
  resize(event->size());
  update();
}

game::LabyrinthMiniGame::Drawer::Drawer(game::LabyrinthMiniGame* mini_game)
    : mini_game_(mini_game) {
  rect_radial = mini_game_->container_->rect();
  radialGrad = QRadialGradient(mini_game_->player_pos_,
                             mini_game_->container_->rect().height() / 2.);
  radialGrad.setColorAt(0, QColor(0, 0, 0, 0));
  // radialGrad.setColorAt(0.5, Qt::black);
  radialGrad.setColorAt(1, Qt::black);
}
