project(demo)
file(GLOB source
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)
add_executable(${PROJECT_NAME} ${source})
target_link_libraries(${PROJECT_NAME}
  geometry-lab::core geometry-lab::render
)