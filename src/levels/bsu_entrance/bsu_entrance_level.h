#ifndef SRC_LEVELS_BSU_ENTRANCE_BSU_ENTRANCE_LEVEL_H_
#define SRC_LEVELS_BSU_ENTRANCE_BSU_ENTRANCE_LEVEL_H_

#include <memory>

#include "../bsu_lobby/guard_behaviour.h"
#include "../bsu_lobby/guard_mini_game.h"
#include "../../ecs/world.h"
#include "../../resources/level.h"
#include "../../resources/behaviour.h"
#include "../../resources/path.h"

namespace game {

class BsuEntranceLevel : public resource::Level {
 public:
  ~BsuEntranceLevel() override;
  void Dispose(ecs::World* world) override;
  void Load(ecs::World* world) override;
  void Process(ecs::World* world, ContextBag) override;
  void CreateMap(const QString& path) override;
  void CreateObject(map::MapLayer layer, const map::MapObject& object) override;
  void CreatePath(resource::Path path, const QString& name) override;

 private:
  ecs::World* world_;
  ecs::Entity* player_ = nullptr;
  QPointF door_pos_;
};

}  // namespace game

#endif  // SRC_LEVELS_BSU_ENTRANCE_BSU_ENTRANCE_LEVEL_H_
