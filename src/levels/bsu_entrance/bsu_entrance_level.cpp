#include "bsu_entrance_level.h"

#include "../../components/components.h"
#include "../../utils/parser/ase_animation_parser.h"
#include "../../map/map_loader.h"
#include "../../context/level_context.h"
#include "../../levels/bsu_lobby/bsu_lobby_level.h"

#include <cmath>

namespace game {

BsuEntranceLevel::~BsuEntranceLevel() = default;

void BsuEntranceLevel::Dispose(ecs::World* world) {
  for (auto& entity : world->ScanEntities<>()) {
    world->EraseEntity(&entity);
  }
}

void BsuEntranceLevel::Load(ecs::World* world) {
  world_ = world;
  map::MapLoader::Load(QFile(":/BSU_entrance.json"), this);
}

void BsuEntranceLevel::Process(ecs::World* world, ContextBag contexts) {
  auto [pl_pos] = player_->Unpack<PositionComponent>();
  if (abs(door_pos_.y() - pl_pos.position.y()) < 22) {  // pls don't comment this :)
    contexts.level_context->Load<BsuLobbyLevel>();
  }
}

void BsuEntranceLevel::CreateMap(const QString& path) {
  resource::Level::CreateMap(world_, path);
}

void BsuEntranceLevel::CreateObject(map::MapLayer layer, const map::MapObject& object) {
  if (layer == map::MapLayer::kCollision) {
    if (object.name == "door") {
      door_pos_ = object.position;
    }
    CreateCollider(world_, object);
    return;
  }
  if (object.name == "player") {
    player_ = &CreatePlayer(world_, object);
  }
}

void BsuEntranceLevel::CreatePath(resource::Path path, const QString& name) {
}

}  // namespace game
