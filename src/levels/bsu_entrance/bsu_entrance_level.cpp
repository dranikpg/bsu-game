#include "bsu_entrance_level.h"

#include "../../components/components.h"
#include "../../utils/parser/ase_animation_parser.h"
#include "../../map/map_loader.h"
#include "../../context/level_context.h"
#include "../../levels/bsu_lobby/bsu_lobby_level.h"
#include "../../constants/keys.h"

#include <cmath>
#include <QDebug>

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
  if (abs(door_pos_.y() - pl_pos.position.y()) < 22 &&
      contexts.input_context->GetFrameKeys().count(constants::Keys::kEnter) > 0) {
    contexts.level_context->Load<BsuLobbyLevel>();
  } else if (std::hypot(pl_pos.position.x() - mini_game_pos_.x(),
                        pl_pos.position.y() - mini_game_pos_.y()) < 100 &&
            state_ == State::kNone &&
            contexts.input_context->GetFrameKeys().count(constants::Keys::kEnter) > 0) {
    StartMiniGame(contexts);
  } else if (state_ == State::kMiniGame) {
    mini_game_->Process();
  } else if (state_ == State::kMiniGameFinished) {
    state_ = State::kNone;
  }
}

void BsuEntranceLevel::CreateMap(const QString& path) {
  resource::Level::CreateMap(world_, path);
}

void BsuEntranceLevel::CreateObject(map::MapLayer layer, const map::MapObject& object) {
  if (layer == map::MapLayer::kCollision) {
    if (object.name == "door") {
      door_pos_ = object.position;
    } else if (object.name == "secret") {
      mini_game_pos_ = object.position;
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

void BsuEntranceLevel::StartMiniGame(ContextBag contexts) {
  contexts.mini_game_context->Start();
  mini_game_ = std::make_shared<SecretMiniGame>(
      [this]() { state_ = State::kMiniGameFinished; },
      contexts.mini_game_context->GetContainer(),
      contexts.input_context, world_);
  state_ = State::kMiniGame;
}

}  // namespace game
