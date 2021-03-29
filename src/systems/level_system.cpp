#include "level_system.h"

#include <QFile>
#include <QDebug>

namespace game {

void LevelSystem::Run(ecs::World* world) {
  if (level) {
    level->Process(world);
  }
  if (loading_level) {
    loading_level->Load(world);
    if (level) {
      level->Dispose(world);
    }
    level = std::move(loading_level);
    loading_level = nullptr;
  }
}

void LevelSystem::LoadLevel(LevelRef level_ref) {
  loading_level = std::move(level_ref);
}

LevelSystem::LevelSystem(context::LevelContext* context)
    : context(context), level(nullptr) {
  context->SetLoadCallback([this](LevelRef ref) {
    this->LoadLevel(std::move(ref));
  });
}

}  // namespace game
