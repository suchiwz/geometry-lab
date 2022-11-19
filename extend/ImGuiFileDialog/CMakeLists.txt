project(filedialog)
message(STATUS "[${PROJECT_NAME}] Fetching...")

FetchContent_Declare(${PROJECT_NAME}
    GIT_REPOSITORY  https://github.com/aiekick/ImGuiFileDialog.git
    GIT_TAG         v0.6.4
)
FetchContent_MakeAvailable(${PROJECT_NAME})
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/ImGuiFileDialogConfig.h.in"
  "${filedialog_SOURCE_DIR}/ImGuiFileDialogConfig.h"
)
file(GLOB source
    "${filedialog_SOURCE_DIR}/*.h"
    "${filedialog_SOURCE_DIR}/*.cpp"
)
add_library(${PROJECT_NAME} ${source})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${filedialog_SOURCE_DIR}
    PUBLIC ${filedialog_SOURCE_DIR}/dirent
    PUBLIC ${filedialog_SOURCE_DIR}/stb
)
target_link_libraries(${PROJECT_NAME} imgui)
message(STATUS "[${PROJECT_NAME}] Done at ${filedialog_SOURCE_DIR}")

