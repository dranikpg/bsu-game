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
  world->CreateEntity().AddComponent<game::PositionComponent>(
          background.width() / 2,
          background.height() / 2)
      .AddComponent<game::SpriteComponent>(background,
                                           SpriteLayer::kBackground);
}

ecs::Entity& resource::Level::CreatePlayer(ecs::World* world,
                                           const map::MapObject& object) {
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
      .AddComponent<game::MovementAnimationSyncComponent>(sync_pack)
      .AddComponent<game::ImpulseComponent>()
      .AddComponent<game::CameraComponent>(1);

  return player;
}

QPointF Level::ProjectToScreen(ecs::World* world,
                               Level::ContextBag contexts,
                               QPointF point) {
  ecs::Entity* camera_entity = world->ScanEntities<game::CameraComponent,
                                                   game::PositionComponent>().Peek();
  auto[camera, camera_pos] = camera_entity->Unpack<game::CameraComponent,
                                                   game::PositionComponent>();
  point -= camera_pos.position;
  point /= camera.scale;
  QSize window_size = contexts.mini_game_context->GetContainer()->size();
  return point + QPointF(window_size.width() / 2, window_size.height() / 2);
}

}  // namespace resource
