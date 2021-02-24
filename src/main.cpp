#include <QApplication>
#include <QLabel>
#include <QListWidget>

#include "ecs/ecs.h"

struct Message : public ecs::Component {
  QString message;
  explicit Message(const QString& message) : message(message) {}
};

struct Visible : public ecs::Component {
};

/**
 * Should add exactly two items: "First visible" and "Second visible"
 */
void TestEcs(QListWidget* list) {
  ecs::World world;

  world.CreateEntity()
      .Create<Message>("First visible")
      .Create<Visible>();

  world.CreateEntity()
      .Create<Message>("Second invisible");

  ecs::Entity& second_visible = world.CreateEntity()
      .Create<Message>("Second visible")
      .Create<Visible>();

  world.CreateEntity()
      .Create<Message>("Second visible")
      .Create<Visible>();

  world.EraseEntity(second_visible);

  world.Run();

  for (auto[message, _] : world.Scan<Message, Visible>()) {
    list->addItem(message.message);
  }
}

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  QListWidget list;
  TestEcs(&list);
  list.show();
  list.resize(300, 100);
  return QApplication::exec();
}
