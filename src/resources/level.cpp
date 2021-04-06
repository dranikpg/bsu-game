#include "level.h"

#include <QPixmap>
#include <QFile>
#include <QDebug>

#include "../components/collider_component.h"
#include "../components/position_component.h"
#include "../components/bounds_component.h"
#include "../components/name_component.h"
#include "../components/sprite_component.h"

using game::ColliderComponent;
using game::PositionComponent;
using game::BoundsComponent;
using game::NameComponent;
using game::AnimationComponent;
using game::SpriteComponent;
using resource::Animation;

void resource::Level::CreateCollider(ecs::World* world,
                                      const map::MapObject* map_object) {
  QPixmap kek("/Users/nikita/CLionProjects/bsu-game/test/4874.jpg");
  world->CreateEntity().AddComponent<ColliderComponent>()
                       .AddComponent<PositionComponent>(map_object->position.x() +
                                                        map_object->size.width() / 2,
                                                        map_object->position.y() +
                                                        map_object->size.height() / 2)
                       .AddComponent<BoundsComponent>(map_object->size.width(),
                                                      map_object->size.height());
}

void resource::Level::CreateMap(ecs::World* world, const QString& path) {
  QPixmap background(path);
  world->CreateEntity().AddComponent<PositionComponent>(QPoint(background.width() / 2,
                                                               background.height() / 2))
                       .AddComponent<SpriteComponent>(background, SpriteLayer::kBackground);
}
