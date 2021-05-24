#ifndef SRC_RESOURCES_LEVEL_H_
#define SRC_RESOURCES_LEVEL_H_

#include "../map/map_instantiator.h"
#include "../map/map_object.h"
#include "../ecs/ecs.h"

#include "../context/mini_game_context.h"
#include "../context/input_context.h"

namespace context {

class LevelContext;

}

namespace resource {

class Level : public map::MapInstantiator {
 public:
  struct ContextBag {
    context::LevelContext* level_context;
    context::InputContext* input_context;
    context::MiniGameContext* mini_game_context;
  };

  virtual ~Level() = default;
  virtual void Process(ecs::World* world, ContextBag) = 0;
  virtual void Load(ecs::World* world) = 0;
  virtual void Dispose(ecs::World* word) = 0;

 protected:
  ecs::Entity& CreatePlayer(ecs::World* world, const map::MapObject& position);
  void CreateMap(ecs::World* world, const QString& path);
  ecs::Entity& CreateCollider(ecs::World* world, const map::MapObject& map_object);
  void CreateCamera(ecs::World* world);

  QPointF ProjectToScreen(ecs::World*, ContextBag, QPointF point);
};

}  // namespace resource

#endif  // SRC_RESOURCES_LEVEL_H_
