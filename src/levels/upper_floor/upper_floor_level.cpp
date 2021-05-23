#include "upper_floor_level.h"

#include <QDebug>

#include "../../map/map_loader.h"
#include "../../map/map_layer.h"

namespace game {

void UpperFloorLevel::CreateMap(const QString& path) {
  Level::CreateMap(world_, path);
}
void UpperFloorLevel::CreateObject(map::MapLayer layer, const map::MapObject& object) {
  if (object.name == "player") {
    CreatePlayer(world_, object);
  }
  if (layer == map::MapLayer::kCollision) {
    CreateCollider(world_, object);
  }
}
void UpperFloorLevel::CreatePath(resource::Path path, const QString& name) {

}
void UpperFloorLevel::Process(ecs::World* world, resource::Level::ContextBag bag) {

}
void UpperFloorLevel::Load(ecs::World* world) {
  world_ = world;
  map::MapLoader::Load(QFile(":/upper_floor.json"), this);
}
void UpperFloorLevel::Dispose(ecs::World* world) {
  for (auto& entity : world->ScanEntities<>()) {
    world->EraseEntity(&entity);
  }
}

}  // namespace game