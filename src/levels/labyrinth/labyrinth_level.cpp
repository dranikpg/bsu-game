#include "labyrinth_level.h"

#include "../../components/components.h"
#include "../../utils/parser/ase_animation_parser.h"
#include "../../map/map_loader.h"
#include "../../context/level_context.h"
#include "../../utils/parser/dialog_parser.h"
#include "../../levels/bsu_entrance/bsu_entrance_level.h"

#include <map>
#include <memory>
#include <utility>
#include <cmath>
#include <QDebug>
#include <QPainter>

namespace game {
LabyrinthLevel::~LabyrinthLevel() = default;

void LabyrinthLevel::Dispose(ecs::World* world) {
  for (auto& entity : world->ScanEntities<>()) {
    world->EraseEntity(&entity);
  }
}

void LabyrinthLevel::Load(ecs::World* world) {
  world_ = world;
  map::MapLoader::Load(QFile(":/labyrinth.json"), this);
}

void LabyrinthLevel::Process(ecs::World* world, ContextBag contexts) {
  if (state_ == State::kNone) {
    StartMiniGame(contexts);
  } else if (state_ == State::kMiniGame) {
    mini_game_->Process(ProjectPlayerPos(world, contexts));
  }
}

void LabyrinthLevel::CreateMap(const QString& path) {
  resource::Level::CreateMap(world_, path);
}

void LabyrinthLevel::CreateObject(map::MapLayer layer, const map::MapObject& object) {
  if (layer == map::MapLayer::kCollision) {
    CreateCollider(world_, object);
    return;
  }
  if (object.name == "player") {
    player_ = &CreatePlayer(world_, object);
  } else if (object.name == "guard") {
    guard_pos_ = object.position;
    CreateGuard(world_, object);
  }
}

void LabyrinthLevel::CreatePath(resource::Path path, const QString& name) {

}

void LabyrinthLevel::StartMiniGame(ContextBag contexts) {
  contexts.mini_game_context->Start();
  mini_game_ = std::make_shared<LabyrinthMiniGame>(
      [this]() { state_ = State::kFinishedMiniGame; },
      contexts.mini_game_context->GetContainer(), contexts.input_context);
  state_ = State::kMiniGame;
}

void LabyrinthLevel::CreateGuard(ecs::World* world,
                                const map::MapObject& object) {
  auto anims = utils::AseAnimationParser::Parse(QFile(":/guard.json"));
  guard_ = &world_->CreateEntity()
      .AddComponent<PositionComponent>(guard_pos_)
      .AddComponent<BoundsComponent>(object.size)
      .AddComponent<ColliderComponent>()
      .AddComponent<SpriteComponent>(QRect(0, -27, 40, 103),
                                     SpriteLayer::kForeground)
      .AddComponent<AnimationComponent>(anims["main"]);
}

QPointF LabyrinthLevel::ProjectPlayerPos(ecs::World* world,
                                        ContextBag contexts) {
  QPointF point = player_->GetComponent<PositionComponent>().position;
  point.ry() -= player_->GetComponent<BoundsComponent>().bounds.height() * 2;
  return ProjectToScreen(world, contexts, point);
}

}  // namespace game


