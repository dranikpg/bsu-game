#include "labyrinth_level.h"

#include "../../components/components.h"
#include "../../utils/parser/ase_animation_parser.h"
#include "../../map/map_loader.h"
#include "../../context/level_context.h"
#include "../../utils/parser/dialog_parser.h"
#include "../../levels/bsu_lobby/bsu_lobby_level.h"
#include "../../levels/bsu_entrance/bsu_entrance_level.h"

#include <map>
#include <memory>
#include <cmath>
#include <QDebug>
#include <QPainter>
#include <QLabel>

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

  is_switched = std::make_shared<bool>(false);
}

void LabyrinthLevel::Process(ecs::World* world, ContextBag contexts) {
  if (state_ == State::kNone) {
    StartMiniGame(contexts);
  } else if (state_ == State::kMiniGame) {
    auto[pl_pos] = player_->Unpack<PositionComponent>();
    if (IsReady(pl_pos.position, pig_, contexts)) {
      QPixmap icon(":/sitnikova.png");
      world_->CreateEntity().AddComponent<game::SplashComponent>(
          utils::PixmapRect(icon, QRect(0, 0, 64, 64), QPoint(2, 2)),
          "The first number is 1",
          []() {
          });
    } else if (IsReady(pl_pos.position, egor_, contexts)) {
      QPixmap icon(":/egor.png");
      world_->CreateEntity().AddComponent<game::SplashComponent>(
          utils::PixmapRect(icon, QRect(0, 0, 64, 64), QPoint(2, 2)),
          "The second number is 4",
          []() {
          });
    } else if (IsReady(pl_pos.position, gora_, contexts)) {
      QPixmap icon(":/gora.png");
      world_->CreateEntity().AddComponent<game::SplashComponent>(
          utils::PixmapRect(icon, QRect(0, 0, 128, 128), QPoint(2, 2)),
          "The third number is 8",
          []() {
          });
    } else if (IsReady(pl_pos.position, guard_pos_, contexts)) {
      QPixmap icon(":/guard-sheet.png");
      world_->CreateEntity().AddComponent<game::SplashComponent>(
          utils::PixmapRect(icon, QRect(0, 0, 64, 64), QPoint(1, 1)),
          "The fourth number is 8",
          []() {
          });
    } else if (IsReady(pl_pos.position, generator_, contexts)) {
      QPixmap icon(":/generator.png");
      world_->CreateEntity().AddComponent<game::SplashComponent>(
          utils::PixmapRect(icon, QRect(0, 0, 64, 64), QPoint(2, 2)),
          "##%@#$#beep~&@~~beep##%%$$",
          [&]() {
            *is_switched = !(*is_switched);
          });
    } else if (IsReady(pl_pos.position, entrance_, contexts)) {
      contexts.mini_game_context->Stop();
      contexts.level_context->Load<BsuLobbyLevel>();
    } else if (IsReady(pl_pos.position, exit_, contexts)) {
      contexts.mini_game_context->Stop();
      contexts.level_context->Load<BsuEntranceLevel>();
    }
    mini_game_->Process(ProjectPlayerPos(world, contexts));
  }
}

void LabyrinthLevel::CreateMap(const QString& path) {
  resource::Level::CreateMap(world_, path);
}

void LabyrinthLevel::CreateObject(map::MapLayer layer, const map::MapObject& object) {
  if (layer == map::MapLayer::kCollision) {
    if (object.name == "entrance") {
      entrance_ = object.position;
    } else if (object.name == "exit") {
      exit_ = object.position;
    }
    CreateCollider(world_, object);
    return;
  }
  if (object.name == "player") {
    player_ = &CreatePlayer(world_, object);
  } else if (object.name == "guard") {
    guard_pos_ = object.position;
    CreateGuard(world_, object);
  } else if (object.name == "sitnikova") {
    pig_ = object.position;
    world_->CreateEntity().AddComponent<PositionComponent>(pig_)
        .AddComponent<BoundsComponent>(object.size)
        .AddComponent<ColliderComponent>()
        .AddComponent<SpriteComponent>(QPixmap(":/sitnikova.png"), SpriteLayer::kForeground);
  } else if (object.name == "egor") {
    egor_ = object.position;
    world_->CreateEntity().AddComponent<PositionComponent>(egor_)
        .AddComponent<BoundsComponent>(object.size)
        .AddComponent<ColliderComponent>()
        .AddComponent<SpriteComponent>(QPixmap(":/egor.png"), SpriteLayer::kForeground);
  } else if (object.name == "gora") {
    gora_ = object.position;
    world_->CreateEntity().AddComponent<PositionComponent>(gora_)
        .AddComponent<BoundsComponent>(object.size)
        .AddComponent<ColliderComponent>()
        .AddComponent<SpriteComponent>(QPixmap(":/gora.png"), SpriteLayer::kForeground);
  } else if (object.name == "generator") {
    generator_ = object.position;
    world_->CreateEntity().AddComponent<PositionComponent>(generator_)
        .AddComponent<BoundsComponent>(object.size)
        .AddComponent<ColliderComponent>()
        .AddComponent<SpriteComponent>(QPixmap(":/generator.png"), SpriteLayer::kForeground);
  }
}

void LabyrinthLevel::CreatePath(resource::Path path, const QString& name) {}

void LabyrinthLevel::StartMiniGame(ContextBag contexts) {
  contexts.mini_game_context->Start();
  mini_game_ = std::make_shared<LabyrinthMiniGame>(
      [this]() { state_ = State::kFinishedMiniGame; },
      contexts.mini_game_context->GetContainer(), contexts.input_context, is_switched);
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
  return ProjectToScreen(world, contexts, point);
}

bool LabyrinthLevel::IsReady(QPointF first, QPointF second, ContextBag context_bag) const {
  return (std::hypot(first.x() - second.x(), first.y() - second.y()) < 40 &&
      context_bag.input_context->GetFrameKeys().count(constants::Keys::kEnter));
}

}  // namespace game
