#include "dialog_system.h"

#include <optional>
#include <map>
#include <utility>

#include <QString>
#include <QDebug>

#include "../components/dialog_component.h"
#include "../constants/keys.h"

namespace game {

void DialogSystem::Run(ecs::World* world) {
  ecs::Entity* dialog_entity = world->ScanEntities<DialogComponent>().Peek();

  bool fresh_dialog = false;
  if (dialog_entity == nullptr) {
    dialog_context_->Hide();
    last_dialog_ = dialog_entity;
    return;
  } else if (dialog_entity != last_dialog_) {
    dialog_context_->Show(QPixmap(), QRect(0, 0, 0, 0));
    fresh_dialog = true;
  }

  auto& dialog = dialog_entity->GetComponent<DialogComponent>();

  if (input_context_->GetFrameKeys().count(constants::Keys::kEnter)
      || fresh_dialog) {
    NextStep(&dialog);
  }

  last_dialog_ = dialog_entity;
}

DialogSystem::DialogSystem(context::InputContext* input_context,
                           context::DialogContext* dialog_context)
    : input_context_(input_context), dialog_context_(dialog_context),
      last_dialog_(nullptr) {}

void DialogSystem::NextStep(DialogComponent* dialog) {
  dialog->current_step++;
  if (dialog->current_step >= dialog->dialog->GetParts().size()) {
    // we're at options step
    if (dialog->current_step < dialog->dialog->GetStepCount()) {
      // shop options
      dialog_context_->SetQuestions(std::map<QString, QString>(),
                                    [this](QString id) {
                                      OptionCallback(std::move(id));
                                    });
    } else {
      dialog_context_->Hide();
      std::optional<QString> chosen = std::nullopt;
      dialog->finish_handler(chosen);
    }
  } else {
    dialog_context_->SetText(dialog->dialog->GetPart(dialog->current_step));
  }
}

void DialogSystem::OptionCallback(QString id) {
  if (last_dialog_ != nullptr) {
    last_dialog_->GetComponent<DialogComponent>()
        .finish_handler(id);
  } else {
    // howd that happen??
  }
}

}  // namespace game
