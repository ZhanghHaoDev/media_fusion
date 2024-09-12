include(FetchContent)

FetchContent_Declare(
  gflags
  GIT_REPOSITORY https://github.com/gflags/gflags.git
  GIT_TAG master  
)

FetchContent_MakeAvailable(gflags)