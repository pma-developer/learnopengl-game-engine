# CMakeList.txt : CMake project for learnopenglmath, include source and define
# project specific logic here.
#
include_directories(../include/)
set(HEADERS  ../inc/headerfilename)
# Add source to this project's executable.
add_executable (learnopenglmath "learnopenglmath.cpp" "learnopenglmath.h"  "Shader.cpp" "Shader.h" "glad.c" "math_utils.cpp" "math_utils.h" "camera.cpp" "camera.h" "global_consts.h")
target_link_libraries(learnopenglmath ${CMAKE_SOURCE_DIR}/lib/glfw3.lib)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET learnopenglmath PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
