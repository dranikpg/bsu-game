#include "bsu_lobby_level.h"

#include "../../components/components.h"
#include "../../utils/parser/ase_animation_parser.h"
#include "../../map/map_loader.h"
#include "../../context/level_context.h"
#include "../../utils/parser/dialog_parser.h"

#include <map>
#include <memory>
#include <utility>
#include <cmath>

namespace game {
BsuLobbyLevel::~BsuLobbyLevel() = default;

void BsuLobbyLevel::Dispose(ecs::World* world) {
  for (auto& entity : world->ScanEntities<>()) {
    world->EraseEntity(&entity);
  }
}

void BsuLobbyLevel::Load(ecs::World* world) {
  world_ = world;
  map::MapLoader::Load(QFile(":/lobby.json"), this);

  guard_behaviour_ = std::make_shared<GuardBehaviour>(player_, guard_path_, guard_pos_);
  guard_->GetComponent<BehaviourComponent>().behaviour = guard_behaviour_;
}

void BsuLobbyLevel::Process(ecs::World* world, ContextBag contexts) {
  if (std::hypotf((canteen_pos_ - player_->GetComponent<PositionComponent>().position).x(),
                  (canteen_pos_ - player_->GetComponent<PositionComponent>().position).y()) < 100 &&
      state_ == State::kNone) {
    StartMiniGame(contexts);
  } else if (state_ == State::kMiniGame) {
    mini_game_->Process();
  } else if (state_ == State::kFinishedMiniGame) {
    contexts.mini_game_context->Stop();
    contexts.level_context->Load<BsuLobbyLevel>();
  }
}

void BsuLobbyLevel::CreateMap(const QString& path) {
  resource::Level::CreateMap(world_, path);
}

void BsuLobbyLevel::CreateObject(map::MapLayer layer, const map::MapObject& object) {
  if (layer == map::MapLayer::kCollision) {
    if (object.name == "block") {
      block_ = &CreateCollider(world_, object);
    } else {
      CreateCollider(world_, object);
    }
    return;
  }
  if (object.name == "player") {
    player_ = &CreatePlayer(world_, object);
  } else if (object.name == "guard") {
    guard_pos_ = object.position;
    CreateGuard(world_, object);
  } else if (object.name == "canteen") {
    canteen_pos_ = object.position;
  }
}

void BsuLobbyLevel::CreatePath(resource::Path path, const QString& name) {
  if (name == "guard_path") {
    guard_path_ = std::make_shared<resource::Path>(std::move(path));
  }
}

void BsuLobbyLevel::StartMiniGame(ContextBag contexts) {
  contexts.mini_game_context->Start();
  mini_game_ = std::make_shared<CanteenMiniGame>(
      [this]() { state_ = State::kFinishedMiniGame; },
      contexts.mini_game_context->GetContainer(), contexts.input_context);
  state_ = State::kMiniGame;
}

void BsuLobbyLevel::CreateGuard(ecs::World* world,
                                const map::MapObject& object) {
  auto anims = utils::AseAnimationParser::Parse(QFile(":/guard.json"));
  std::multimap<constants::AnimationType,
                std::shared_ptr<resource::Animation>> sync_pack;
  sync_pack.insert(std::make_pair(AnimationType::kDefault, anims["main"]));
  sync_pack.insert(std::make_pair(AnimationType::kLeft, anims["move"]));
  sync_pack.insert(std::make_pair(AnimationType::kRight, anims["move"]));
  guard_ = &world_->CreateEntity()
      .AddComponent<PositionComponent>(guard_pos_)
      .AddComponent<BoundsComponent>(object.size)
      .AddComponent<ColliderComponent>()
      .AddComponent<ImpulseComponent>()
      .AddComponent<SpriteComponent>(QRect(0, -27, 40, 103),
                                     SpriteLayer::kForeground)
      .AddComponent<BehaviourComponent>(nullptr)
      .AddComponent<AnimationComponent>(anims["main"])
      .AddComponent<MovementAnimationSyncComponent>(sync_pack)
      .AddComponent<PathFollowComponent>(
          resource::Path(object.position),
          constants::PathFollowType::kOnce,
          1);
}

QPointF BsuLobbyLevel::ProjectPlayerPos(ecs::World* world,
                                        ContextBag contexts) {
  QPointF point = player_->GetComponent<PositionComponent>().position;
  point.ry() -= player_->GetComponent<BoundsComponent>().bounds.height() * 2;
  return ProjectToScreen(world, contexts, point);
}

}  // namespace game
