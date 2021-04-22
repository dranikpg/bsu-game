#ifndef SRC_LEVELS_BSU_ENTRANCE_BSU_ENTRANCE_LEVEL_H_
#define SRC_LEVELS_BSU_ENTRANCE_BSU_ENTRANCE_LEVEL_H_

#include <memory>

#include "guard_behaviour.h"
#include "guard_mini_game.h"
#include "../../ecs/world.h"
#include "../../resources/level.h"
#include "../../resources/behaviour.h"
#include "../../resources/path.h"

namespace game {

class BsuEntranceLevel : public resource::Level {
 public:
  ~BsuEntranceLevel() override;
  void Dispose(ecs::World* word) override;
  void Load(ecs::World* world) override;
  void Process(ecs::World* world, ContextBag) override;
  void CreateMap(const QString& path) override;
  void CreateObject(map::MapLayer layer, const map::MapObject& object) override;
  void CreatePath(resource::Path path, const QString& name) override;

 private:
  void CreateGuard(ecs::World* world, const map::MapObject& object);
  void StartMiniGame(ContextBag contexts);

 private:
  enum class State {
    kNone,
    kMiniGame,
    kFinished
  };

  State state_;
  ecs::World* world_;
  ecs::Entity* player_ = nullptr;
  ecs::Entity* guard_ = nullptr;
  std::shared_ptr<GuardMiniGame> mini_game_;
  std::shared_ptr<resource::Path> guard_path_;
  std::shared_ptr<GuardBehaviour> guard_behaviour_;
  QPointF guard_pos_;
};

}  // namespace game

#endif  // SRC_LEVELS_BSU_ENTRANCE_BSU_ENTRANCE_LEVEL_H_
