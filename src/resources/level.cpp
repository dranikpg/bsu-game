#include "level.h"

#include <map>
#include <memory>
#include <utility>

#include <QPixmap>
#include <QFile>

#include "../components/components.h"
#include "../utils/parser/ase_animation_parser.h"

namespace resource {

ecs::Entity& resource::Level::CreateCollider(ecs::World* world,
                                             const map::MapObject& map_object) {
  return world->CreateEntity().AddComponent<game::ColliderComponent>()
      .AddComponent<game::PositionComponent>(map_object.position.x(),
                                             map_object.position.y())
      .AddComponent<game::BoundsComponent>(map_object.size.width(),
                                           map_object.size.height());
}

void resource::Level::CreateMap(ecs::World* world, const QString& path) {
  QPixmap background(path);
  world->CreateEntity()
      .AddComponent<game::PositionComponent>(background.width() / 2, background.height() / 2)
      .AddComponent<game::SpriteComponent>(background, SpriteLayer::kBackground)
      .AddComponent<game::BoundsComponent>(background.width(), background.height())
      .AddComponent<game::MapComponent>();
}

ecs::Entity& resource::Level::CreatePlayer(ecs::World* world,
                                           const map::MapObject& object) {
  CreateCamera(world);

  auto anims = utils::AseAnimationParser::Parse(QFile(":/player.json"));

  using constants::AnimationType;
  std::multimap<AnimationType, std::shared_ptr<resource::Animation>> sync_pack;
  sync_pack.insert(std::make_pair(AnimationType::kDefault, anims["main"]));
  sync_pack.insert(std::make_pair(AnimationType::kLeft, anims["left"]));
  sync_pack.insert(std::make_pair(AnimationType::kRight, anims["right"]));
  sync_pack.insert(std::make_pair(AnimationType::kDown, anims["down"]));
  sync_pack.insert(std::make_pair(AnimationType::kUp, anims["up"]));

  ecs::Entity& player = world->CreateEntity()
      .AddComponent<game::PositionComponent>(object.position)
      .AddComponent<game::BoundsComponent>(object.size.width(),
                                           object.size.height())
      .AddComponent<game::ColliderComponent>()
      .AddComponent<game::InputMovementComponent>()
      .AddComponent<game::SpriteComponent>(QRect(0, -16, 64, 64),
                                           SpriteLayer::kForeground)
      .AddComponent<game::AnimationComponent>(anims["main"])
      .AddComponent<game::MovementAnimationSyncComponent>(sync_pack, 1024)
      .AddComponent<game::ImpulseComponent>()
      .AddComponent<game::CameraFollowComponent>();
  return player;
}

QPointF Level::ProjectToScreen(ecs::World* world,
                               Level::ContextBag contexts,
                               QPointF point) {
  return contexts.mini_game_context->GetRenderTransform().map(point);
}

void Level::CreateCamera(ecs::World* world) {
  world->CreateEntity()
      .AddComponent<game::PositionComponent>(0, 0)
      .AddComponent<game::CameraComponent>(1.5f);
}

}  // namespace resource
