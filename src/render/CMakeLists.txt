project(render)
set(INC_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
file(GLOB source
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.in"
)
add_library(${PROJECT_NAME} ${source})
target_include_directories(${PROJECT_NAME} 
  PUBLIC ${INC_PATH}
)
target_link_libraries(${PROJECT_NAME}
  PUBLIC glad glfw glm imgui core
)
add_library(geometry-lab::render ALIAS ${PROJECT_NAME})