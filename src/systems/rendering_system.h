#ifndef SRC_SYSTEMS_RENDERING_SYSTEM_H_
#define SRC_SYSTEMS_RENDERING_SYSTEM_H_

#include "../ecs/ecs.h"
#include "../context/painter_context.h"
#include "../context/window_context.h"

using ecs::World;
using ecs::System;
using context::PainterContext;
using context::WindowContext;

namespace game {

class RenderingSystem : public System {
 public:
  RenderingSystem(PainterContext* painter_context, WindowContext* window_context);
  void Run(World* world) override;

 private:
  PainterContext* painter_context_;
  WindowContext* window_context_;
};

}  //  namespace game

#endif  // SRC_SYSTEMS_RENDERING_SYSTEM_H_
