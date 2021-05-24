#include "upper_floor_level.h"

#include "../../components/components.h"
#include "../../map/map_loader.h"
#include "../../map/map_layer.h"

#include <cmath>

namespace game {

void UpperFloorLevel::CreateMap(const QString& path) {
  Level::CreateMap(world_, path);
}
void UpperFloorLevel::CreateObject(map::MapLayer layer, const map::MapObject& object) {
  if (object.name == "player") {
    player_ = &CreatePlayer(world_, object);
  } else if (object.name == "math") {
    math_marker_ = object.position;
  } else if (object.name == "lab") {
    lab_marker_ = object.position;
  }
  if (layer == map::MapLayer::kCollision) {
    CreateCollider(world_, object);
  }
}
void UpperFloorLevel::CreatePath(resource::Path path, const QString& name) {

}
void UpperFloorLevel::Process(ecs::World* world, resource::Level::ContextBag bag) {
  auto& pos = player_->GetComponent<PositionComponent>().position;
  if (state_ != State::MinigameMath && std::hypotf(math_marker_.x() - pos.x(), math_marker_.y() - pos.y()) < 50) {
    StartMinigameMath(bag);
  } else if (state_ != State::MinigameLab && std::hypotf(lab_marker_.x() - pos.x(), lab_marker_.y() - pos.y()) < 50) {
    StartMinigameMath(bag);
  } else if (state_ == State::MinigameMath) {
    minigame_math_->Process();
  } else if (state_ == State::MinigameLab) {
    minigame_lab_->Process();
  }
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

void UpperFloorLevel::StartMinigameMath(resource::Level::ContextBag contexts) {
  minigame_math_ = std::make_shared<ChernovMiniGame>([this](){}, contexts.mini_game_context->GetContainer(), world_);
  state_ = State::MinigameMath;
}

void UpperFloorLevel::StartMinigameLab(resource::Level::ContextBag contexts) {
  minigame_lab_ = std::make_shared<SecretMiniGame>([this](){}, contexts.mini_game_context->GetContainer(), contexts.input_context, world_);
  state_ = State::MinigameLab;
}

}  // namespace game