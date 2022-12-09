project(openmesh)
message(STATUS "[${PROJECT_NAME}] Fetching...")
FetchContent_Declare(openmesh
  GIT_REPOSITORY  https://gitlab.vci.rwth-aachen.de:9000/OpenMesh/OpenMesh.git
  GIT_TAG         OpenMesh-9.0
  GIT_PROGRESS    TRUE
)
set(BUILD_APPS OFF CACHE BOOL "" FORCE)
set(OPENMESH_DOCS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(${PROJECT_NAME})
message(STATUS "[${PROJECT_NAME}] Down at ${openmesh_SOURCE_DIR}")

