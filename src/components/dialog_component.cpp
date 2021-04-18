#include "dialog_component.h"

namespace game {

DialogComponent::DialogComponent(const std::shared_ptr<resource::Dialog>& dialog,
                                 const DialogComponent::FinishHandler& finish_handler)
    : dialog(dialog), finish_handler(finish_handler), current_step(-1) {}

}  // namespace game
