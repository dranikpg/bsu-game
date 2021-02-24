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
  SyncEntities();
  for (Entity* entity: entities_) {
    delete entity;
  }
}

Entity& World::CreateEntity() {
  auto* e = new Entity();
  entities_created_.insert(e);
  return *e;
}

void World::EraseEntity(const Entity& es_ptr) {
  // this cast is only required to type-case the pointer
  // it does not perform any modifications
  auto* ptr = const_cast<Entity*>(&es_ptr);
  assert(ptr != nullptr);
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
  for (Entity* entity: entities_created_) {
    entities_.insert(entity);
  }
  for (Entity* entity: entities_deleted_) {
    delete entity;
  }
  entities_created_.clear();
  entities_deleted_.clear();
}

}  // namespace ecs
