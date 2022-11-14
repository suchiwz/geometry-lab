project(imgui)
message(STATUS "[${PROJECT_NAME}] Fetching...")
FetchContent_Declare(${PROJECT_NAME}
  GIT_REPOSITORY  https://github.com/ocornut/imgui.git
  GIT_TAG         v1.88
)
FetchContent_MakeAvailable(${PROJECT_NAME})
file(GLOB source
    "${imgui_SOURCE_DIR}/*.h"
    "${imgui_SOURCE_DIR}/*.cpp"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.*"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.*"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3_loader.*"
)
add_library(${PROJECT_NAME} ${source})

target_include_directories(${PROJECT_NAME}
    PUBLIC "${imgui_SOURCE_DIR}"
    PUBLIC "${imgui_SOURCE_DIR}/backends"
)
target_link_libraries(${PROJECT_NAME} glfw)

message(STATUS "[${PROJECT_NAME}] Done at ${imgui_SOURCE_DIR}")