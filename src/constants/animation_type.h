#ifndef SRC_CONSTANTS_ANIMATION_TYPE_H_
#define SRC_CONSTANTS_ANIMATION_TYPE_H_

namespace constants {

enum class AnimationType {
  kShiftUp,
  kShiftDown,
  kShiftRight,
  kShiftLeft,
  kShiftLeftDownCorner,
  kShiftRightDownCorner,
  kShiftLeftUpCorner,
  kShiftRightUpCorner,
  kDefault
};

}  // namespace constants

#endif  // SRC_CONSTANTS_ANIMATION_TYPE_H_
