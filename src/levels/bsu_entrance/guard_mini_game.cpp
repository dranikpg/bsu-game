#include "guard_mini_game.h"

#include <QWidget>
#include <QPainter>
#include <QResizeEvent>
#include <QPen>

namespace game {


void game::GuardMiniGame::Process(QPointF player_pos) {
  nimbus_->SetPlayerPos(player_pos);
  if (timer_++ > 120) {
    callback_();
  }
}

game::GuardMiniGame::GuardMiniGame(const game::GuardMiniGame::Callback& callback,
                                   QWidget* container)
    : callback_(callback) {
  nimbus_ = new NimbusDrawer();
  nimbus_->setParent(container);
  nimbus_->resize(container->size());
  nimbus_->show();
}

void GuardMiniGame::NimbusDrawer::paintEvent(QPaintEvent* event) {
  QPainter painter(this);
  QPen pen;
  pen.setStyle(Qt::DashDotLine);
  pen.setWidth(3);
  pen.setBrush(Qt::yellow);
  pen.setCapStyle(Qt::RoundCap);
  pen.setJoinStyle(Qt::RoundJoin);

  painter.setPen(pen);
  painter.drawEllipse(player_pos_, 20, 10);
}

void GuardMiniGame::NimbusDrawer::SetPlayerPos(const QPointF& player_pos) {
  player_pos_ = player_pos;
}

void GuardMiniGame::NimbusDrawer::resizeEvent(QResizeEvent* event) {
  resize(event->size());
}

}  // namespace game
