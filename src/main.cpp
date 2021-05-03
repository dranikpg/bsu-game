#include <QApplication>

#include "prototype/prototype_widget.h"

void LoadStyles(QApplication* app) {
  QFile file(":/style.css");
  file.open(QFile::ReadOnly);
  QString styleSheet = QLatin1String(file.readAll());
  app->setStyleSheet(styleSheet);
}

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  LoadStyles(&app);

  PrototypeWidget prototype;
  prototype.show();
  prototype.resize(500, 500);
  return QApplication::exec();
}
