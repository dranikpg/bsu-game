#include "example_level.h"

#include "../../map/map_loader.h"

namespace level {

void ExampleLevel::Process(ecs::World* world) {
}

void ExampleLevel::CreateMap(const QString& path) {
  qDebug() << "requested to create map!";
  Level::CreateMap(world_, path);
}
void ExampleLevel::CreateObject(map::MapLayer layer,
                                const map::MapObject& object) {
  qDebug() << "object: " << object.name;
}
void ExampleLevel::CreatePath(const resources::Path& path,
                              const QString& name) {
  qDebug() << "path of size: " << path.Size();
}

ExampleLevel::~ExampleLevel() {
}

void ExampleLevel::Load(ecs::World* world) {
  world_ = world;
  map::MapLoader::Load(QFile("test/tiled_test.json"), this);
}

void ExampleLevel::Dispose(ecs::World* word) {
}

}  // namespace level
