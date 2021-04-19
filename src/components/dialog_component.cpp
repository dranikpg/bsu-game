#include "dialog_component.h"

#include <utility>

namespace game {

DialogComponent::DialogComponent(const std::shared_ptr<resource::Dialog>& dialog,
                                 DialogComponent::FinishHandler finish_handler)
    : dialog(dialog),
      finish_handler(std::move(finish_handler)),
      current_step(-1) {}

DialogComponent::DialogComponent(const std::shared_ptr<resource::Dialog>& dialog,
                                 DialogComponent::FinishHandler finish_handler,
                                 utils::PixRect icon)
    : dialog(dialog),
      finish_handler(std::move(finish_handler)),
      current_step(-1),
      icon(icon) {}

void DialogComponent::Finish() {
  current_step = dialog->GetStepCount() + 1;
}

}  // namespace game
