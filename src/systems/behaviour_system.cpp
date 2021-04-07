#include "behaviour_system.h"

#include "../components/behaviour_component.h"

void game::BehaviourSystem::Run(ecs::World* world) {
  for (auto& entity : world->ScanEntities<game::BehaviourComponent>()) {
    auto [behaviour] = entity.Unpack<game::BehaviourComponent>();
    if (behaviour.behaviour == nullptr) {
      continue;
    }
    behaviour.behaviour->Process(&entity);
  }
}
