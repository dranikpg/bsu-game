#ifndef SRC_LEVELS_BSU_ENTRANCE_BSU_ENTRANCE_LEVEL_H_
#define SRC_LEVELS_BSU_ENTRANCE_BSU_ENTRANCE_LEVEL_H_

#include <memory>

#include "../bsu_lobby/guard_behaviour.h"
#include "../bsu_lobby/guard_mini_game.h"
#include "../../ecs/world.h"
#include "../../resources/level.h"
#include "../../resources/behaviour.h"
#include "../../resources/path.h"
#include "../../levels/bsu_entrance/secret_mini_game.h"

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
  void StartMiniGame(ContextBag contexts);

 private:
  enum class State {
    kNone,
    kMiniGame,
    kMiniGameFinished
  };

  ecs::World* world_;
  ecs::Entity* player_ = nullptr;
  QPointF door_pos_;
  QPointF mini_game_pos_;
  std::shared_ptr<SecretMiniGame> mini_game_ = nullptr;
  State state_ = State::kNone;
};

}  // namespace game

#endif  // SRC_LEVELS_BSU_ENTRANCE_BSU_ENTRANCE_LEVEL_H_
