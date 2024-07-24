cmake_minimum_required(VERSION 3.21)
include(FetchContent)

project(PacmanGame C CXX)

add_executable(PacmanGame "${CMAKE_SOURCE_DIR}/main.cpp")

set_target_properties(PacmanGame PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)

SET(CXX_WARN_OPTIONS "-Waddress -Warray-bounds -Wc++0x-compat -Wchar-subscripts -Wcomment -Wmissing-braces -Wformat -Wparentheses -Wreorder -Wreturn-type -Wsequence-point -Wsign-compare -Wstrict-aliasing -Wstrict-overflow=1 -Wswitch -Wtrigraphs -Wuninitialized -Wunknown-pragmas -Wunused-label -Wunused-value -Wunused-variable -Wvolatile-register-var -Wextra -Woverloaded-virtual -Wno-deprecated -Wno-invalid-offsetof -Wno-placement-new")

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})
target_link_libraries(PacmanGame PRIVATE ${OPENGL_LIBRARY})

target_include_directories(PacmanGame PRIVATE "${CMAKE_SOURCE_DIR}")
target_include_directories(PacmanGame PRIVATE "${CMAKE_SOURCE_DIR}/ext/glad/include")

add_library(GLAD "ext/glad/src/glad.c")
target_include_directories(GLAD PRIVATE "${CMAKE_SOURCE_DIR}/ext/glad/include")

#SDL2
FetchContent_Declare(
  SDL2
  URL https://github.com/libsdl-org/SDL/releases/download/release-2.30.5/SDL2-2.30.5.tar.gz
)
FetchContent_MakeAvailable(SDL2)

# GLM
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
)
FetchContent_MakeAvailable(glm)

target_link_libraries(PacmanGame PRIVATE
  SDL2::SDL2-static
  glm::glm
  GLAD
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})