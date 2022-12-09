project(glm)
message(STATUS "[${PROJECT_NAME}] Fetching...")
FetchContent_Declare(${PROJECT_NAME}
  GIT_REPOSITORY  https://github.com/g-truc/glm.git
  GIT_TAG         0.9.9.8
  GIT_PROGRESS    TRUE
)
FetchContent_MakeAvailable(${PROJECT_NAME})
message(STATUS "[${PROJECT_NAME}] Done at ${glm_SOURCE_DIR}")