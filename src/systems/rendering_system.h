#ifndef SRC_SYSTEMS_RENDERING_SYSTEM_H_
#define SRC_SYSTEMS_RENDERING_SYSTEM_H_

#include "../ecs/ecs.h"
#include "../context/painter_context.h"

using ecs::World;
using ecs::System;
using context::PainterContext;

namespace game {

class RenderingSystem : public System {
 public:
  explicit RenderingSystem(PainterContext* painter);
  void Run(World *world) override;
 private:
  PainterContext* painter_context_;
};

}  //  namespace game

#endif  // SRC_SYSTEMS_RENDERING_SYSTEM_H_
