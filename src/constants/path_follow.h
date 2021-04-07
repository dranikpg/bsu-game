#ifndef SRC_CONSTANTS_PATH_FOLLOW_H_
#define SRC_CONSTANTS_PATH_FOLLOW_H_

namespace constants {

enum class PathFollowType {
  kOnce,
  kLoop,
  kClosedLoop
};

enum class PathFollowState{
  kResolvingMoving,
  kResolvingWaiting,
  kWaiting,
  kMoving,
  kFinished
};

}  // namespace constants

#endif  // SRC_CONSTANTS_PATH_FOLLOW_H_
