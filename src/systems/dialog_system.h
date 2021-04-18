#ifndef SRC_SYSTEMS_DIALOG_SYSTEM_H_
#define SRC_SYSTEMS_DIALOG_SYSTEM_H_

#include "../ecs/ecs.h"
#include "../components/dialog_component.h"
#include "../context/dialog_context.h"
#include "../context/input_context.h"

namespace game {

class DialogSystem : public ecs::System {
 public:
  DialogSystem(context::InputContext* input_context,
               context::DialogContext* dialog_context);
  void Run(ecs::World* world) override;
 private:
  void NextStep(DialogComponent* dialog);
  void OptionCallback(QString id);
 private:
  context::InputContext* input_context_;
  context::DialogContext* dialog_context_;
  ecs::Entity* last_dialog_;
};

}  // namespace game

#endif  // SRC_SYSTEMS_DIALOG_SYSTEM_H_
