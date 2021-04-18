#ifndef SRC_COMPONENTS_DIALOG_COMPONENT_H_
#define SRC_COMPONENTS_DIALOG_COMPONENT_H_

#include <memory>

#include "../ecs/component.h"
#include "../resources/dialog.h"

namespace game {

struct DialogComponent : public ecs::Component {
  using FinishHandler = std::function<void(std::optional<QString> opt)>;

  DialogComponent(const std::shared_ptr<resource::Dialog>& dialog,
                  const FinishHandler& finish_handler);

  std::shared_ptr<resource::Dialog> dialog;
  FinishHandler finish_handler;
  int current_step;
};

}  // namespace game

#endif  // SRC_COMPONENTS_DIALOG_COMPONENT_H_
