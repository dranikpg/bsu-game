#include "example_level.h"

#include "../../map/map_loader.h"

namespace level {

void ExampleLevel::Process(ecs::World* world) {
  // processing game logic
}

void ExampleLevel::CreateMap(const QString& path) {
  qDebug() << "requested to create map!";
  Level::CreateMap(world_, path);
}
void ExampleLevel::CreateObject(map::MapLayer layer,
                                const map::MapObject& object) {
  qDebug() << "object: " << object.name;
}
void ExampleLevel::CreatePath(const resource::Path& path,
                              const QString& name) {
  qDebug() << "path of size: " << path.Size();
}

ExampleLevel::~ExampleLevel() {
}

void ExampleLevel::Load(ecs::World* world) {
  world_ = world;
  // start loading here via MapLoader
}

void ExampleLevel::Dispose(ecs::World* word) {
  // delete all created entities
}

}  // namespace level
