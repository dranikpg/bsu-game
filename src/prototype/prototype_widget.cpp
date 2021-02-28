#include "prototype_widget.h"

#include <vector>
#include <utility>
#include <memory>

#include <QPainter>


PrototypeWidget::PrototypeWidget() {
  std::vector<std::unique_ptr<System>> systems;
  systems.emplace_back(std::make_unique<game::RenderingSystem>(&painter_context_));
  world_.Init(std::move(systems));
}

void PrototypeWidget::paintEvent(QPaintEvent* event) {
  QWidget::paintEvent(event);

  QPainter painter(this);
  painter_context_.SetPainter(&painter);

  world_.Run();
  update();
}
