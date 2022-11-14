project(eigen)
message(STATUS "[${PROJECT_NAME}] Fetching...")
FetchContent_Declare(${PROJECT_NAME}
  GIT_REPOSITORY  https://gitlab.com/libeigen/eigen.git
  GIT_TAG         3.4.0
)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(EIGEN_BUILD_DOC OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(${PROJECT_NAME})
message(STATUS "[${PROJECT_NAME}] Down at ${eigen_SOURCE_DIR}")

