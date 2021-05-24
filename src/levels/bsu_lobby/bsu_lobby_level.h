#ifndef SRC_LEVELS_BSU_LOBBY_BSU_LOBBY_LEVEL_H_
#define SRC_LEVELS_BSU_LOBBY_BSU_LOBBY_LEVEL_H_

#include <memory>

#include "../bsu_lobby/guard_behaviour.h"
#include "../bsu_lobby/guard_mini_game.h"
#include "../../ecs/world.h"
#include "../../resources/level.h"
#include "../../resources/behaviour.h"
#include "../../levels/bsu_lobby/canteen_mini_game.h"

//debug
#include "../bsu_sixth_floor/chernov_mini_game/chernov_mini_game.h"

#include "../../resources/path.h"

namespace game {

class BsuLobbyLevel : public resource::Level {
 public:
  ~BsuLobbyLevel() override;
  void Dispose(ecs::World* world) override;
  void Load(ecs::World* world) override;
  void Process(ecs::World* world, ContextBag) override;
  void CreateMap(const QString& path) override;
  void CreateObject(map::MapLayer layer, const map::MapObject& object) override;
  void CreatePath(resource::Path path, const QString& name) override;

 private:
  void CreateGuard(ecs::World* world, const map::MapObject& object);
  void CreateCanteen(ecs::World* world, const map::MapObject& object);
  void StartMiniGame(ContextBag contexts);
  QPointF ProjectPlayerPos(ecs::World* world, ContextBag contexts);

 private:
  enum class State {
    kNone,
    kMiniGame,
    kFinishedMiniGame,
    kFinishedDialog
  };

  State state_;
  ecs::World* world_;
  ecs::Entity* player_ = nullptr;
  ecs::Entity* guard_ = nullptr;
  ecs::Entity* block_ = nullptr;
  ecs::Entity* canteen_;
  //debug
  std::shared_ptr<ChernovMiniGame> mini_game_;
  std::shared_ptr<resource::Path> guard_path_;
  std::shared_ptr<GuardBehaviour> guard_behaviour_;
  QPointF guard_pos_;
  QPointF canteen_pos_;
  utils::PixmapRect canteen_icon_;
};

}  // namespace game

#endif  // SRC_LEVELS_BSU_LOBBY_BSU_LOBBY_LEVEL_H_
