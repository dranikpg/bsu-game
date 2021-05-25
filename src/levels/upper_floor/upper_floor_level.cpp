#include "upper_floor_level.h"

#include "../../components/components.h"
#include "../../map/map_loader.h"
#include "../../map/map_layer.h"
#include "../../utils/splash.h"
#include "../../utils/parser/ase_animation_parser.h"

#include <cmath>

#include <QDebug>
#include <QRectF>

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
  } else if (object.name == "students") {
    CreateStudents(object);
  }
  if (layer == map::MapLayer::kCollision) {
    CreateCollider(world_, object);
  }
}
void UpperFloorLevel::CreatePath(resource::Path path, const QString& name) {}

void UpperFloorLevel::Process(ecs::World* world, resource::Level::ContextBag bag) {
  if (state_ == State::Halted) {
    return;
  }

  auto& pos = player_->GetComponent<PositionComponent>().position;
  if (state_ != State::MinigameMath
      && std::hypotf(math_marker_.x() - pos.x(), math_marker_.y() - pos.y()) < 20) {
    StartMinigameMath(bag);
  } else if (state_ != State::MinigameLab
      && std::hypotf(lab_marker_.x() - pos.x(), lab_marker_.y() - pos.y()) < 50) {
    StartMinigameLab(bag);
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
  minigame_math_ = std::make_shared<ChernovMiniGame>(
      [this]() {state_ = State::None;}, contexts.mini_game_context->GetContainer(), world_);
  auto splash = utils::Splash::Load("math_minigame");
  world_->CreateEntity()
      .AddComponent<SplashComponent>(
          splash.first,
          splash.second,
          [this]() {
            state_ = State::MinigameMath;
          });
  state_ = State::Halted;
}

void UpperFloorLevel::StartMinigameLab(resource::Level::ContextBag contexts) {
  qDebug() << "clm";
  minigame_lab_ = std::make_shared<SecretMiniGame>(
      [this]() {state_ = State::None;}, contexts.mini_game_context->GetContainer(), contexts.input_context, world_);
  auto splash = utils::Splash::Load("lab_minigame");
  world_->CreateEntity()
      .AddComponent<SplashComponent>(
          splash.first,
          splash.second,
          [this]() {
            state_ = State::MinigameLab;
          });
  state_ = State::Halted;
}

void UpperFloorLevel::CreateStudents(const map::MapObject& object) {
  auto anim_bag = utils::AseAnimationParser::Parse(QFile(":/students.json"));
  world_->CreateEntity()
      .AddComponent<PositionComponent>(object.position)
      .AddComponent<BoundsComponent>(64, 64)
      .AddComponent<ColliderComponent>()
      .AddComponent<SpriteComponent>(QRectF(0, 0, 100, 100))
      .AddComponent<AnimationComponent>(anim_bag["main"]);
}

}  // namespace game
