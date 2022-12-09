project(glfw)
message(STATUS "[${PROJECT_NAME}] Fetching...")
FetchContent_Declare(${PROJECT_NAME}
  GIT_REPOSITORY  https://github.com/glfw/glfw.git
  GIT_TAG         3.3.8
  GIT_PROGRESS    TRUE
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(${PROJECT_NAME})
message(STATUS "[${PROJECT_NAME}] Done at ${glfw_SOURCE_DIR}")