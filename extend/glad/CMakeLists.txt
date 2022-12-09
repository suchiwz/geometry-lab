project(glad)
message(STATUS "[${PROJECT_NAME}] Fetching...")
FetchContent_Declare(${PROJECT_NAME}
  GIT_REPOSITORY  https://github.com/suchiwz/glad.git
  GIT_TAG main
  GIT_PROGRESS    TRUE
)
FetchContent_MakeAvailable(${PROJECT_NAME})
file(GLOB source
  "${glad_SOURCE_DIR}/src/*.c"
)
add_library(${PROJECT_NAME} ${source})
target_include_directories(${PROJECT_NAME}
  PUBLIC "${glad_SOURCE_DIR}/include"
)
message(STATUS "[${PROJECT_NAME}] Done at ${glad_SOURCE_DIR}")