#ifndef SRC_LEVELS_EXAMPLE_EXAMPLE_LEVEL_H_
#define SRC_LEVELS_EXAMPLE_EXAMPLE_LEVEL_H_

#include "../../resources/level.h"

namespace level {

class ExampleLevel : public resource::Level {
 public:
  ~ExampleLevel() override;
  void Dispose(ecs::World* word) override;
  void Load(ecs::World* world) override;
  void Process(ecs::World* world) override;
  void CreateMap(const QString& path) override;
  void CreateObject(map::MapLayer layer, const map::MapObject& object) override;
  void CreatePath(const resource::Path& path, const QString& name) override;
 private:
  ecs::World* world_;
};

}  // namespace level

#endif  // SRC_LEVELS_EXAMPLE_EXAMPLE_LEVEL_H_
