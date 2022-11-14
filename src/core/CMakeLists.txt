project(core)
set(INC_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
file(GLOB source
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)
add_library(${PROJECT_NAME} ${source})
target_include_directories(${PROJECT_NAME} 
  PUBLIC ${INC_PATH}
)
target_link_libraries(${PROJECT_NAME}
  PUBLIC OpenMeshCore OpenMeshTools eigen
)
add_library(geometry-lab::core ALIAS ${PROJECT_NAME})
