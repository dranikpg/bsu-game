#include "level.h"

#include <QPixmap>
#include <QFile>
#include <QDebug>

#include "../components/components.h"
#include "../utils/parser/ase_animation_parser.h"

namespace resource {

void resource::Level::CreateCollider(ecs::World* world,
                                     const map::MapObject& map_object) {
  using namespace game;
  world->CreateEntity().AddComponent<ColliderComponent>()
      .AddComponent<PositionComponent>(map_object.position.x(),
                                       map_object.position.y())
      .AddComponent<BoundsComponent>(map_object.size.width(),
                                     map_object.size.height());
}

void resource::Level::CreateMap(ecs::World* world, const QString& path) {
  using namespace game;
  QPixmap background(path);
  world->CreateEntity().AddComponent<PositionComponent>(QPoint(
          background.width() / 2,
          background.height() / 2))
      .AddComponent<SpriteComponent>(background, SpriteLayer::kBackground);
}

ecs::Entity& resource::Level::CreatePlayer(ecs::World* world, const map::MapObject& object) {
  using namespace game;
  auto anims = utils::AseAnimationParser::Parse(QFile(":/player.json"));

  using constants::AnimationType;
  std::multimap<AnimationType, std::shared_ptr<resource::Animation>> sync_pack;
  sync_pack.insert(std::make_pair(AnimationType::kDefault, anims["main"]));
  sync_pack.insert(std::make_pair(AnimationType::kLeft, anims["left"]));
  sync_pack.insert(std::make_pair(AnimationType::kRight, anims["right"]));
  sync_pack.insert(std::make_pair(AnimationType::kDown, anims["down"]));
  sync_pack.insert(std::make_pair(AnimationType::kUp, anims["up"]));

  ecs::Entity& player = world->CreateEntity()
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

  return player;
}

}