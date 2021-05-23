#ifndef BSU_GAME_SRC_LEVELS_UPPER_FLOOR_UPPER_FLOOR_LEVEL_H_
#define BSU_GAME_SRC_LEVELS_UPPER_FLOOR_UPPER_FLOOR_LEVEL_H_

#include "../../ecs/ecs.h"
#include "../../resources/level.h"

namespace game {

class UpperFloorLevel : public resource::Level {
 public:
  void CreateMap(const QString& path) override;
  void CreateObject(map::MapLayer layer, const map::MapObject& object) override;
  void CreatePath(resource::Path path, const QString& name) override;
  void Process(ecs::World* world, ContextBag bag) override;
  void Load(ecs::World* world) override;
  void Dispose(ecs::World* word) override;
 public:
  ecs::World* world_;
};

}  // namespace game

#endif //BSU_GAME_SRC_LEVELS_UPPER_FLOOR_UPPER_FLOOR_LEVEL_H_
