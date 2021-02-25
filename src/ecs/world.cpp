#include <cassert>

#include "world.h"
#include "entity.h"
#include "system.h"

namespace ecs {

void World::Init(const std::vector<System*>& systems) {
  for (System* system: systems) {
    systems_.push_back(std::unique_ptr<System>(system));
  }
}

World::~World() {
}
Entity& World::CreateEntity() {
  auto entity = std::make_shared<Entity>();
  entities_created_.insert(entity);
  return *entity;
}

void World::EraseEntity(const Entity& const_reference) {
  auto& mut_reference = const_cast<Entity&>(const_reference);
  std::shared_ptr<Entity> ptr = mut_reference.shared_from_this();
  if (entities_.count(ptr)) {
    entities_deleted_.push_back(ptr);
    entities_.erase(ptr);
  } else if (entities_created_.count(ptr)) {
    entities_deleted_.push_back(ptr);
    entities_created_.erase(ptr);
  }
}

void World::Run() {
  SyncEntities();
  for (auto& sys: systems_) {
    sys->Run(this);
  }
}

void World::SyncEntities() {
  for (std::shared_ptr<Entity> entity: entities_created_) {
    entities_.insert(entity);
  }
  entities_created_.clear();
  entities_deleted_.clear();
}

}  // namespace ecs
