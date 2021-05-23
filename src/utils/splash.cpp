#include "splash.h"

#include <QFile>

namespace utils {

std::pair<PixmapRect, QString> Splash::Load(const std::string& name) {
  std::string path = ":/splashes/"+name;
  QFile textFile(QString::fromStdString(path + ".txt"));
  if (!textFile.open(QFile::ReadOnly | QFile::Text)) {
    throw std::runtime_error("Failed to read file");
  }
  QString splash_text = textFile.readAll();
  QPixmap splash_pixmap(QString::fromStdString(path + ".png"));
  return std::make_pair(PixmapRect(splash_pixmap), std::move(splash_text));
}

}  // namespace utils