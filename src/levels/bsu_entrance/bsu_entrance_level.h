#ifndef SRC_LEVELS_BSU_ENTRANCE_BSU_ENTRANCE_LEVEL_H_
#define SRC_LEVELS_BSU_ENTRANCE_BSU_ENTRANCE_LEVEL_H_

#include "../../resources/level.h"
#include "../../ecs/world.h"
#include "../../resources/behaviour.h"

namespace level {

class BsuEntranceLevel : public resource::Level {
 public:
  ~BsuEntranceLevel() override;
  void Dispose(ecs::World* word) override;
  void Load(ecs::World* world) override;
  void Process(ecs::World* world) override;
  void CreateMap(const QString& path) override;
  void CreateObject(map::MapLayer layer, const map::MapObject& object) override;
  void CreatePath(const resource::Path& path, const QString& name) override;
 private:
  ecs::World* world_;
  ecs::Entity* player_ = nullptr;
  ecs::Entity* guard_ = nullptr;
  QPoint door_pos_;
};

class GuardBehaviour : public resource::Behaviour {
 public:
  GuardBehaviour() = default;
  GuardBehaviour(ecs::Entity*, QPoint, QPoint);
  void Process(ecs::Entity* entity) override;
  bool IsCloseToDoor(QPoint guard_pos) const;

 private:
  ecs::Entity* player_ = nullptr;
  QPoint door_pos_;
  QPoint main_position_;
};
}  // namespace level
#endif  // SRC_LEVELS_BSU_ENTRANCE_BSU_ENTRANCE_LEVEL_H_
