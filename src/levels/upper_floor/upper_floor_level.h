#ifndef BSU_GAME_SRC_LEVELS_UPPER_FLOOR_UPPER_FLOOR_LEVEL_H_
#define BSU_GAME_SRC_LEVELS_UPPER_FLOOR_UPPER_FLOOR_LEVEL_H_

#include <memory>

#include <QPointF>

#include "../../ecs/ecs.h"
#include "../../resources/level.h"
#include "chernov_mini_game.h"
#include "secret_mini_game.h"

namespace game {

class UpperFloorLevel : public resource::Level {
 public:
  void CreateMap(const QString& path) override;
  void CreateObject(map::MapLayer layer, const map::MapObject& object) override;
  void CreatePath(resource::Path path, const QString& name) override;
  void Process(ecs::World* world, ContextBag bag) override;
  void Load(ecs::World* world) override;
  void Dispose(ecs::World* word) override;
 private:
  void StartMinigameMath(ContextBag contexts);
  void StartMinigameLab(ContextBag contexts);
 private:
  enum class State {
    None,
    MinigameMath,
    MinigameLab
  };
  State state_ = State::None;
  QPointF math_marker_;
  std::shared_ptr<ChernovMiniGame> minigame_math_;
  QPointF lab_marker_;
  std::shared_ptr<SecretMiniGame> minigame_lab_;
  ecs::World* world_;
  ecs::Entity* player_;
};

}  // namespace game

#endif //BSU_GAME_SRC_LEVELS_UPPER_FLOOR_UPPER_FLOOR_LEVEL_H_
