#ifndef SRC_RESOURCES_LEVEL_H_
#define SRC_RESOURCES_LEVEL_H_

#include "../map/map_instantiator.h"
#include "../map/map_object.h"
#include "../ecs/world.h"

namespace resource {

class Level : public map::MapInstantiator {
 public:
  virtual ~Level() = default;
  virtual void Process(ecs::World* world) = 0;
  virtual void Load(ecs::World* world) = 0;
  virtual void Dispose(ecs::World* word) = 0;

 protected:
  void CreateMap(ecs::World* world, const QString& path);
  void CreateCollider(ecs::World* world, const map::MapObject* map_object);
};

}  // namespace resource

#endif  // SRC_RESOURCES_LEVEL_H_
