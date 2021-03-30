#include "level_system.h"

#include <QFile>

namespace game {

void LevelSystem::Run(ecs::World* world) {
  if (level_) {
    level_->Process(world);
  }
  if (loading_level_) {
    loading_level_->Load(world);
    if (level_) {
      level_->Dispose(world);
    }
    level_ = std::move(loading_level_);
    loading_level_ = nullptr;
  }
}

void LevelSystem::LoadLevel(LevelRef level_ref) {
  loading_level_ = std::move(level_ref);
}

LevelSystem::LevelSystem(context::LevelContext* context)
    : context_(context), level_(nullptr) {
  context->SetOnLevelCreated([this](LevelRef ref) {
    this->LoadLevel(std::move(ref));
  });
}

}  // namespace game
