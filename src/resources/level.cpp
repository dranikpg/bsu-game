#include "level.h"

#include <QPixmap>
#include <QFile>
#include <QDebug>

#include "../components/collider_component.h"
#include "../components/position_component.h"
#include "../components/bounds_component.h"
#include "../components/sprite_component.h"

using game::ColliderComponent;
using game::PositionComponent;
using game::BoundsComponent;
using game::SpriteComponent;


void resource::Level::CreateCollider(ecs::World* world,
                                      const map::MapObject* map_object) {
  world->CreateEntity().AddComponent<ColliderComponent>()
                       .AddComponent<PositionComponent>(map_object->position.x(),
                                                        map_object->position.y())
                       .AddComponent<BoundsComponent>(map_object->size.width(),
                                                      map_object->size.height());
}

void resource::Level::CreateMap(ecs::World* world, const QString& path) {
  QPixmap background(path);
  world->CreateEntity().AddComponent<PositionComponent>(QPoint(background.width() / 2,
                                                               background.height() / 2))
                       .AddComponent<SpriteComponent>(background, SpriteLayer::kBackground);
}
