#
# Specify minimum CMake version and project name
#
cmake_minimum_required (VERSION 3.0)
project (ManifoldModelling)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#
# CMake setup
#
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set (CMAKE_VERBOSE_MAKEFILE 0) # 1 should be used for debugging
set (CMAKE_SUPPRESS_REGENERATION TRUE) # Suppresses ZERO_CHECK
if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++11")
  if(NOT WIN32)
    set(GLAD_LIBRARIES dl)
  endif()
endif()

#
# GLFW options
#
option (GLFW_INSTALL OFF)
option (GLFW_BUILD_DOCS OFF)
option (GLFW_BUILD_EXAMPLES OFF)
option (GLFW_BUILD_TESTS OFF)
add_subdirectory (vendor/glfw)

#
# OpenGL Library
#
set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)

#
# Assimp options
#
set (ASSIMP_INSTALL OFF)
set (ASSIMP_BUILD_DOCS OFF)
set (ASSIMP_BUILD_EXAMPLES OFF)
set (ASSIMP_BUILD_TESTS OFF)
set (ASSIMP_WARNINGS_AS_ERRORS OFF)
add_subdirectory (vendor/assimp)
set (ASSIMP_LIBRARIES assimp)

#
# Add paths
#
set(RESOURCE_DIR "${CMAKE_SOURCE_DIR}/resources")
set(SHADER_DIR "${RESOURCE_DIR}/shaders")
set(TEXTURE_DIR "${RESOURCE_DIR}/textures")

add_definitions(-DRESOURCE_DIR="${RESOURCE_DIR}")
add_definitions(-DSHADER_DIR="${SHADER_DIR}")
add_definitions(-DTEXTURE_DIR="${TEXTURE_DIR}")
#
# Set include paths
#

# Include sources
include_directories (src/
                     src/App
                     src/Shaders
                     src/Renderer
                     src/Math
                     src/Utils
                     src/Models
                     src/Scene)

# Include libs
include_directories (vendor/glad/include/
                     vendor/glfw/include/
                     vendor/glm/
                     vendor/stb/
                     vendor/assimp/include/)

#
# Add files
#
file (GLOB         VENDORS_SOURCES vendor/glad/src/glad.c)
file (GLOB_RECURSE PROJECT_HEADERS src/*.hpp
                                   src/*.h)
file (GLOB_RECURSE PROJECT_SOURCES src/*.cpp
                                   src/*.cxx
                                   src/*.cc
                                   src/*.c)
file (GLOB_RECURSE PROJECT_SHADERS resources/shaders/*.comp
                                   resources/shaders/*.frag
                                   resources/shaders/*.geom
                                   resources/shaders/*.vert)
file (GLOB         PROJECT_CONFIGS CMakeLists.txt
                                   README.md
                                  .gitignore
                                  .gitmodules)

#
# Organizing files
#
source_group ("headers" FILES ${PROJECT_HEADERS})
source_group ("shaders" FILES ${PROJECT_SHADERS})
source_group ("sources" FILES ${PROJECT_SOURCES})
source_group ("vendors" FILES ${VENDORS_SOURCES})

#
# Set executable and target link libraries
#
add_definitions (-DGLFW_INCLUDE_NONE
                 -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")
add_executable (${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
                                ${PROJECT_SHADERS} ${PROJECT_CONFIGS}
                                ${VENDORS_SOURCES})
target_link_libraries (${PROJECT_NAME}
                       glfw
                       OpenGL::GL
                       ${GLFW_LIBRARIES}
                       ${GLAD_LIBRARIES}
                       ${ASSIMP_LIBRARIES})
set_target_properties (${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

