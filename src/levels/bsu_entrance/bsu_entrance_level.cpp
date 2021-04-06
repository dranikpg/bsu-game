#include "bsu_entrance_level.h"

#include "../../components/bounds_component.h"
#include "../../components/position_component.h"
#include "../../components/collider_component.h"
#include "../../components/input_movement_component.h"
#include "../../components/impulse_component.h"
#include "../../components/movement_animation_sync_component.h"
#include "../../components/animation_component.h"
#include "../../components/sprite_component.h"
#include "../../components/camera_component.h"
#include "../../components/behaviour_component.h"
#include "../../utils/parser/ase_animation_parser.h"
#include "../../map/map_loader.h"

#include <map>
#include <memory>
#include <utility>

using game::BoundsComponent;
using game::PositionComponent;
using game::ColliderComponent;
using game::InputMovementComponent;
using game::AnimationComponent;
using game::MovementAnimationSyncComponent;
using game::SpriteComponent;
using game::ImpulseComponent;
using game::CameraComponent;
using game::BehaviourComponent;

level::BsuEntranceLevel::~BsuEntranceLevel() = default;

void level::BsuEntranceLevel::Dispose(ecs::World* world) {
  for (auto& entity : world->ScanEntities<>()) {
    world->EraseEntity(&entity);
  }
}

void level::BsuEntranceLevel::Load(ecs::World* world) {
  world_ = world;
  map::MapLoader::Load(QFile(":/BSU_entrance.json"), this);
  auto [behaviour, guard_pos] = guard_->Unpack<BehaviourComponent, PositionComponent>();
  std::shared_ptr<GuardBehaviour> guard_behaviour =
      std::make_shared<GuardBehaviour>(player_, guard_pos.position, door_pos_);
  behaviour.behaviour = guard_behaviour;
}

void level::BsuEntranceLevel::Process(ecs::World* world) {}

void level::BsuEntranceLevel::CreateMap(const QString& path) {
  resource::Level::CreateMap(world_, path);
}

void level::BsuEntranceLevel::CreateObject(map::MapLayer layer, const map::MapObject& object) {
  if (layer == map::MapLayer::kCollision) {
    if (object.name == "door") {
      door_pos_.setX(object.position.x() + object.size.width() / 2);
      door_pos_.setY(object.position.y() + object.size.height() / 2);
    }
    CreateCollider(world_, &object);
    return;
  }
  if (object.name == "chel") {
    auto anims = utils::AseAnimationParser::Parse(QFile(":/player.json"));
    std::multimap<constants::AnimationType, std::shared_ptr<resource::Animation>> sync_pack;
    sync_pack.insert(std::make_pair(AnimationType::kDefault, anims["main"]));
    sync_pack.insert(std::make_pair(AnimationType::kLeft, anims["left"]));
    sync_pack.insert(std::make_pair(AnimationType::kRight, anims["right"]));
    sync_pack.insert(std::make_pair(AnimationType::kDown, anims["down"]));
    sync_pack.insert(std::make_pair(AnimationType::kUp, anims["up"]));

    player_ = &world_->CreateEntity()
        .AddComponent<PositionComponent>(object.position)
        .AddComponent<BoundsComponent>(object.size.width(), object.size.height())
        .AddComponent<ColliderComponent>()
        .AddComponent<InputMovementComponent>()
        .AddComponent<game::SpriteComponent>(QRect(0, -16, 64, 64),
                                             SpriteLayer::kForeground)
        .AddComponent<AnimationComponent>(anims["main"])
        .AddComponent<MovementAnimationSyncComponent>(sync_pack)
        .AddComponent<ImpulseComponent>()
        .AddComponent<CameraComponent>();
  } else if (object.name == "guard") {
    auto anims = utils::AseAnimationParser::Parse(QFile(":/guard.json"));
    std::multimap<constants::AnimationType, std::shared_ptr<resource::Animation>> sync_pack;
    sync_pack.insert(std::make_pair(AnimationType::kDefault, anims["pokoy"]));
    sync_pack.insert(std::make_pair(AnimationType::kLeft, anims["move"]));
    sync_pack.insert(std::make_pair(AnimationType::kRight, anims["move"]));
    guard_ = &world_->CreateEntity()
        .AddComponent<PositionComponent>(object.position.x() +
                                             object.size.width() / 2,
                                         object.position.y() +
                                             object.size.height() / 2)
        .AddComponent<BoundsComponent>(object.size.width(), object.size.height())
        .AddComponent<ColliderComponent>()
        .AddComponent<ImpulseComponent>()
        .AddComponent<SpriteComponent>(QRect(0, -27, 40, 103),
                                       SpriteLayer::kForeground)
        .AddComponent<BehaviourComponent>(nullptr)
        .AddComponent<AnimationComponent>(anims["pokoy"])
        .AddComponent<MovementAnimationSyncComponent>(sync_pack);
  }
}

void level::BsuEntranceLevel::CreatePath(const resource::Path& path, const QString& name) {}

level::GuardBehaviour::GuardBehaviour(ecs::Entity* player, QPoint main_pos, QPoint door_pos) :
    player_(player),
    main_position_(main_pos),
    door_pos_(door_pos) {}


void level::GuardBehaviour::Process(ecs::Entity* entity) {
  auto [player_pos] = player_->Unpack<PositionComponent>();
  int distance_to_door = sqrt((player_pos.position.x() - door_pos_.x()) *
                         (player_pos.position.x() - door_pos_.x()) +
                         (player_pos.position.y() - door_pos_.y()) *
                         (player_pos.position.y() - door_pos_.y()));

  auto [guard_pos, impulse] = entity->Unpack<PositionComponent, ImpulseComponent>();
  if (distance_to_door <= 180 && !IsCloseToDoor(guard_pos.position)) {
    impulse.shift.setX(-8);
  } else if (distance_to_door <= 180 && IsCloseToDoor(guard_pos.position)) {
    impulse.shift.setX(0);
  } else {
    if (guard_pos.position == main_position_) {
      impulse.shift.setX(0);
      return;
    }
    impulse.shift.setX(8);
  }
}

bool level::GuardBehaviour::IsCloseToDoor(QPoint guard_pos) const {
  return (guard_pos.x() - door_pos_.x()) * (guard_pos.x() - door_pos_.x()) < 100;
}
