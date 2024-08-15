# CMakeList.txt : CMake project for learnopenglGameEngine, include source and define
# project specific logic here.
#
include_directories(../include/)
set(HEADERS  ../inc/headerfilename)

# Add source to this project's executable.
add_executable (learnopenglGameEngine "learnopengl_game_engine.cpp" "learnopengl_game_engine.h"  "Shader.cpp" "Shader.h" "glad.c" "math_utils.cpp" "math_utils.h" "camera.cpp" "camera.h" "global_consts.h" "texture.cpp" "texture.h" "models.h")
target_link_libraries(learnopenglGameEngine ${CMAKE_SOURCE_DIR}/lib/glfw3.lib)
target_link_libraries(learnopenglGameEngine ${CMAKE_SOURCE_DIR}/lib/debug/assimp-vc143-mtd.lib)

add_custom_command(
  TARGET learnopenglGameEngine
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
          ${CMAKE_SOURCE_DIR}/shared-libs/debug/assimp-vc143-mtd.dll
          $<TARGET_FILE_DIR:learnopenglGameEngine>
)
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET learnopenglGameEngine PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
