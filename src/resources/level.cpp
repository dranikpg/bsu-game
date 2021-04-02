#include "level.h"

#include <QPixmap>
#include <QFile>
#include <QDebug>

#include "../components/collider_component.h"
#include "../components/position_component.h"
#include "../components/bounds_component.h"
#include "../components/name_component.h"
#include "../resources/animation.h"
#include "../components/animation_component.h"
#include "../components/sprite_component.h"
#include "../components/position_component.h"
#include "../components/camera_component.h"

using game::ColliderComponent;
using game::PositionComponent;
using game::BoundsComponent;
using game::NameComponent;
using game::AnimationComponent;
using game::SpriteComponent;
using resource::Animation;
using game::CameraComponent;

void resources::Level::CreateCollider(ecs::World* world,
                                      const map::MapObject* map_object) {
  QPixmap kek("/Users/nikita/CLionProjects/bsu-game/test/4874.jpg");
  world->CreateEntity().AddComponent<ColliderComponent>()
                       .AddComponent<PositionComponent>(map_object->position.x() +
                                                        map_object->size.width() / 2,
                                                        map_object->position.y() +
                                                        map_object->size.height() / 2 - 18)
                       .AddComponent<NameComponent>(map_object->name)
                       .AddComponent<BoundsComponent>(map_object->size.width(),
                                                      map_object->size.height());
}

void resources::Level::CreateMap(ecs::World* world, const QString& path) {
  QPixmap background(path);
  world->CreateEntity().AddComponent<PositionComponent>(489, 447)
      .AddComponent<SpriteComponent>(background, SpriteLayer::kBackground);
}
