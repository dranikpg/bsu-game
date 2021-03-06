#include "world.h"

namespace ecs {

void World::Init(std::vector<std::unique_ptr<System>>&& systems) {
  systems_ = std::move(systems);
}

Entity& World::CreateEntity() {
  auto entity = std::make_shared<Entity>();
  entities_created_.insert(entity);
  return *entity;
}

void World::EraseEntity(Entity* raw_ptr) {
  std::shared_ptr<Entity> ptr = raw_ptr->shared_from_this();
  if (entities_.count(ptr)) {
    entities_deleted_.push_back(ptr);
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
  for (const std::shared_ptr<Entity>& entity: entities_created_) {
    entities_.insert(entity);
  }
  for (const std::shared_ptr<Entity>& entity: entities_deleted_) {
    if (entities_.count(entity)) {
      entities_.erase(entity);
    }
  }
  entities_created_.clear();
  entities_deleted_.clear();
}

}  // namespace ecs
