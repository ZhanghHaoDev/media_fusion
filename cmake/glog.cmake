include(FetchContent)

FetchContent_Declare(
  glog
  GIT_REPOSITORY https://github.com/google/glog.git
  GIT_TAG master
)

FetchContent_MakeAvailable(glog)