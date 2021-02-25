#include <cassert>

#include "world.h"
#include "entity.h"
#include "system.h"

namespace ecs {

void World::Init(std::vector<std::unique_ptr<System>>&& systems) {
  systems_ = std::move(systems);
}

World::~World() {
}
Entity& World::CreateEntity() {
  auto entity = std::make_shared<Entity>();
  entities_created_.insert(entity);
  return *entity;
}

void World::EraseEntity(Entity* raw_ptr) {
  assert(raw_ptr != nullptr);
  std::shared_ptr<Entity> ptr = raw_ptr->shared_from_this();
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
  for (const std::shared_ptr<Entity>& entity: entities_created_) {
    entities_.insert(entity);
  }
  entities_created_.clear();
  entities_deleted_.clear();
}

}  // namespace ecs
