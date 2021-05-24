#include "level_system.h"

#include <QFile>

namespace game {

void LevelSystem::Run(ecs::World* world) {
  if (level_) {
    mini_game_context_->SetRenderTransform(window_context_->GetRenderTransform());
    level_->Process(world, {level_context_, input_context_, mini_game_context_});
  }
  if (loading_level_) {
    if (level_) {
      level_->Dispose(world);
    }
    loading_level_->Load(world);
    level_ = std::move(loading_level_);
    loading_level_ = nullptr;
  }
}

void LevelSystem::LoadLevel(LevelRef level_ref) {
  loading_level_ = std::move(level_ref);
}
LevelSystem::LevelSystem(context::LevelContext* level_context,
                         context::MiniGameContext* mini_game_context,
                         context::InputContext* input_context,
                         context::WindowContext* window_context)
    : level_context_(level_context),
      mini_game_context_(mini_game_context),
      input_context_(input_context),
      window_context_(window_context) {
  level_context_->SetOnLevelCreated([this](LevelRef ref) {
    this->LoadLevel(std::move(ref));
  });
}


}  // namespace game
