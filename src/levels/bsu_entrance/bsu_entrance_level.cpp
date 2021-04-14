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
#include "../../levels/bsu_entrance/guard_behaviour.h"

#include <map>
#include <memory>
#include <utility>
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
  auto[behaviour, guard_pos] = guard_->Unpack<BehaviourComponent,
                                              PositionComponent>();
  auto guard_behaviour =
      std::make_shared<GuardBehaviour>(player_, guard_pos.position, door_pos_);
  behaviour.behaviour = guard_behaviour;
}

void BsuEntranceLevel::Process(ecs::World* world) {
}

void BsuEntranceLevel::CreateMap(const QString& path) {
  resource::Level::CreateMap(world_, path);
}

void BsuEntranceLevel::CreateObject(map::MapLayer layer,
                                           const map::MapObject& object) {
  if (layer == map::MapLayer::kCollision) {
    if (object.name == "door") {
      door_pos_.setX(object.position.x());
      door_pos_.setY(object.position.y());
    }
    CreateCollider(world_, object);
    return;
  }
  if (object.name == "chel") {
    player_ = &CreatePlayer(world_, object);
  } else if (object.name == "guard") {
    CreateGuard(world_, object);
  }
}

void BsuEntranceLevel::CreatePath(const resource::Path& path,
                                         const QString& name) {}

void BsuEntranceLevel::CreateGuard(ecs::World* world, const map::MapObject& object) {
  auto anims = utils::AseAnimationParser::Parse(QFile(":/guard.json"));
  std::multimap<constants::AnimationType,
                std::shared_ptr<resource::Animation>> sync_pack;
  sync_pack.insert(std::make_pair(AnimationType::kDefault, anims["main"]));
  sync_pack.insert(std::make_pair(AnimationType::kLeft, anims["move"]));
  sync_pack.insert(std::make_pair(AnimationType::kRight, anims["move"]));
  guard_ = &world_->CreateEntity()
      .AddComponent<PositionComponent>(object.position.x(),
                                       object.position.y())
      .AddComponent<BoundsComponent>(object.size.width(),
                                     object.size.height())
      .AddComponent<ColliderComponent>()
      .AddComponent<ImpulseComponent>()
      .AddComponent<SpriteComponent>(QRect(0, -27, 40, 103),
                                     SpriteLayer::kForeground)
      .AddComponent<BehaviourComponent>(nullptr)
      .AddComponent<AnimationComponent>(anims["main"])
      .AddComponent<MovementAnimationSyncComponent>(sync_pack);
}

}  // namespace game