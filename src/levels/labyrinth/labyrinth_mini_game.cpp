#include "labyrinth_mini_game.h"

#include <QResizeEvent>
#include <QPainter>
#include <utility>
#include <random>
#include <QRegion>
#include <QRect>
#include <QDebug>

void game::LabyrinthMiniGame::Process(QPointF player_pos) {
  player_pos_ = player_pos;
}

game::LabyrinthMiniGame::LabyrinthMiniGame(game::LabyrinthMiniGame::Callback callback,
                                       QWidget* container,
                                       context::InputContext* input) :
                                        callback_(std::move(callback)),
                                        input_(input),
                                        container_(container) {
  container_ = container;
  drawer_ = new Drawer(this);
  drawer_->setParent(container);
  drawer_->show();
}

void game::LabyrinthMiniGame::Drawer::paintEvent(QPaintEvent* event) {
  QPixmap test = QPixmap(":/canteen.png");
  //
  // QRegion r1(QRect(mini_game_->player_pos_.x() - 100,
  //                  mini_game_->player_pos_.y() - 100,200, 200),    // r1: elliptic region
  //           QRegion::Ellipse);
  // QRegion r2(mini_game_->container_->rect());
  // QRegion r3 = r2.subtracted(r1);
  QPainter painter(this);
  painter.drawRect(mini_game_->player_pos_.x() - 5 , mini_game_->player_pos_.y() - 5, 10, 10);
  // painter.setClipRegion(r3);
}

void game::LabyrinthMiniGame::Drawer::resizeEvent(QResizeEvent* event) {
  resize(event->size());
  update();
}

game::LabyrinthMiniGame::Drawer::Drawer(game::LabyrinthMiniGame* mini_game)
    : mini_game_(mini_game) {
}

