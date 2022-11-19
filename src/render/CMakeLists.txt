project(render)
set(GEOMETRY_LAB_SHADER_PATH 
    "${CMAKE_CURRENT_SOURCE_DIR}/shader" CACHE PATH "shader path")
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/render_config.hpp.in"
  "${CMAKE_CURRENT_SOURCE_DIR}/render_config.hpp"
)
set(INC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/..")
file(GLOB source
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.in"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.frag"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.vert"
)
add_library(${PROJECT_NAME} ${source})
target_include_directories(${PROJECT_NAME} 
  PUBLIC ${INC_PATH}
)
target_link_libraries(${PROJECT_NAME}
  PUBLIC glad glfw glm imgui core
)
add_library(geometry-lab::render ALIAS ${PROJECT_NAME})