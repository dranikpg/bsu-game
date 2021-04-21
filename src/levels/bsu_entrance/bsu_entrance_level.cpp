#include "bsu_entrance_level.h"

#include "../../components/components.h"
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

  guard_->GetComponent<BehaviourComponent>().behaviour
    = std::make_shared<GuardBehaviour>(player_, guard_path_, guard_pos_);
}

void BsuEntranceLevel::Process(ecs::World* world) {
}

void BsuEntranceLevel::CreateMap(const QString& path) {
  resource::Level::CreateMap(world_, path);
}

void BsuEntranceLevel::CreateObject(map::MapLayer layer,
                                           const map::MapObject& object) {
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

void BsuEntranceLevel::CreatePath(resource::Path path,
                                         const QString& name) {
  if (name == "guard_path") {
    guard_path_ = std::make_shared<resource::Path>(std::move(path));
  }
}

void BsuEntranceLevel::CreateGuard(ecs::World* world, const map::MapObject& object) {
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
          resource::Path(object.position, object.position),
          constants::PathFollowType::kOnce,
          1);
}

}  // namespace game
