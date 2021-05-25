#include "bsu_lobby_level.h"

#include "../../components/components.h"
#include "../../utils/parser/ase_animation_parser.h"
#include "../../map/map_loader.h"
#include "../../context/level_context.h"
#include "../../utils/parser/dialog_parser.h"
#include "../../utils/splash.h"
#include "../../levels/bsu_entrance/bsu_entrance_level.h"
#include "../../levels/labyrinth/labyrinth_level.h"

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
  if (state_ == State::kHalted) {
    return;
  }

  auto& pos = player_->GetComponent<PositionComponent>().position;
  if (std::hypotf((canteen_pos_ - pos).x(), (canteen_pos_ - pos).y()) < 100 &&
      (state_ == State::kNone || state_ == State::kUnlocked) &&
      contexts.input_context->GetFrameKeys().count(constants::Keys::kEnter)) {
    StartMiniGame(contexts);
  } else if (state_ == State::kMiniGame) {
    mini_game_->Process();
    if (std::hypotf((canteen_pos_ - pos).x(), (canteen_pos_ - pos).y()) > 100) {
      state_ = State::kNone;
    }
  } else if (state_ == State::kFinishedMiniGame) {
    contexts.mini_game_context->Stop();
    guard_behaviour_->AllowPass();
    world->EraseEntity(block_);
    state_ = State::kUnlocked;
  } else if (state_ == State::kUnlocked
      && std::hypotf(transition_pos_.x() - pos.x(), transition_pos_.y() - pos.y()) < 100) {
    contexts.level_context->Load<LabyrinthLevel>();
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
  } else if (object.name == "transition") {
    transition_pos_ = object.position;
  } else if (object.name == "girl") {
    CreateGirl(object);
  }
}

void BsuLobbyLevel::CreatePath(resource::Path path, const QString& name) {
  if (name == "guard_path") {
    guard_path_ = std::make_shared<resource::Path>(std::move(path));
  }
}

void BsuLobbyLevel::StartMiniGame(ContextBag contexts) {
  mini_game_ = std::make_shared<CanteenMiniGame>(
      [this]() { state_ = State::kFinishedMiniGame; },
      contexts.mini_game_context->GetContainer(), contexts.input_context);

  auto splash = utils::Splash::Load("canteen");

  world_->CreateEntity()
    .AddComponent<SplashComponent>(
        splash.first,
        splash.second,
        [this, contexts](){
          state_ = State::kMiniGame;
          contexts.mini_game_context->Start();
        });

  state_ = State::kHalted;
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

void BsuLobbyLevel::CreateGirl(const map::MapObject& object) {
  auto anim_bag = utils::AseAnimationParser::Parse(QFile(":/girl.json"));
  world_->CreateEntity()
      .AddComponent<PositionComponent>(object.position)
      .AddComponent<BoundsComponent>(96, 64)
      .AddComponent<ColliderComponent>()
      .AddComponent<SpriteComponent>(QRectF(0, 0, 96, 105))
      .AddComponent<AnimationComponent>(anim_bag["main"]);
}

}  // namespace game
