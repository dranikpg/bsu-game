#include <QApplication>
#include <QLabel>

int bigFunction() {
}

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  QLabel greeting("BSU GAME");
  greeting.show();
  greeting.resize(300, 100);
  return QApplication::exec();
}
