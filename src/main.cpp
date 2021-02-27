#include <QApplication>

#include "prototype/prototype_widget.h"

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  PrototypeWidget prototype;
  prototype.show();
  prototype.resize(300, 100);
  return QApplication::exec();
}
