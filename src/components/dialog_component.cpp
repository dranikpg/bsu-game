#include "dialog_component.h"

#include <utility>

namespace game {

DialogComponent::DialogComponent(std::shared_ptr<resource::Dialog>  dialog,
                                 DialogComponent::FinishHandler finish_handler)
    : dialog(std::move(dialog)),
      finish_handler(std::move(finish_handler)),
      current_step(-1) {}

DialogComponent::DialogComponent(std::shared_ptr<resource::Dialog>  dialog,
                                 DialogComponent::FinishHandler finish_handler,
                                 const utils::PixmapRect& icon)
    : dialog(std::move(dialog)),
      finish_handler(std::move(finish_handler)),
      current_step(-1),
      icon(icon) {}

void DialogComponent::Finish() {
  current_step = dialog->GetStepCount() + 1;
}

}  // namespace game
