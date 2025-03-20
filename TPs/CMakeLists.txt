# CMake entry point
cmake_minimum_required (VERSION 3.0)
project (Tutorials)

find_package(OpenGL REQUIRED)

if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like bin_Visual2012_64bits/" )
endif()
if( CMAKE_SOURCE_DIR MATCHES " " )
    message( "Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
    message( "Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()

# Compile external dependencies 
add_subdirectory (external)

# On Visual 2005 and above, this module can set the debug working directory
cmake_policy(SET CMP0026 OLD)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/rpavlik-cmake-modules-fe2273")
include(CreateLaunchers)
include(MSVCMultipleProcessCompile) # /MP

if(INCLUDE_DISTRIB)
    add_subdirectory(distrib)
endif(INCLUDE_DISTRIB)

include_directories(
    external/glfw-3.1.2/include/
    external/glm-0.9.7.1/
    external/glew-1.13.0/include/
    external/imgui/
    external/imgui/backends/ 
    external/ImGuiFileDialog
    common/
    .
)

set(ALL_LIBS
    ${OPENGL_LIBRARY}
    glfw
    GLEW_1130
)

# Exclude problematic files causing duplicate symbols
file(GLOB_RECURSE COMMON_SOURCES "common/*.cpp" "common/*.hpp")
list(FILTER COMMON_SOURCES EXCLUDE REGEX ".*objloaderbase\\.cpp$")
list(FILTER COMMON_SOURCES EXCLUDE REGEX ".*stb_image\\.cpp$")  # Exclude stb_image.cpp from glob

# Add stb_image separately to control its inclusion
set(STB_IMAGE_SOURCE "${CMAKE_SOURCE_DIR}/common/stb_image.cpp")

# Define STB_IMAGE_IMPLEMENTATION only in stb_image.cpp
set_source_files_properties(${STB_IMAGE_SOURCE} PROPERTIES
    COMPILE_DEFINITIONS "STB_IMAGE_IMPLEMENTATION")

file(GLOB_RECURSE SRC_SOURCES 
    "src/*.cpp" 
    "src/*.hpp"
    "src/*.h"
    "src/GameObjects/*.cpp" 
    "src/GameObjects/*.hpp"
    "src/GameObjects/Components/*.cpp" 
    "src/GameObjects/Components/*.hpp"
    "src/Rendering/*.cpp" 
    "src/Rendering/*.hpp"
    "src/Ressources/*.cpp" 
    "src/Ressources/*.hpp"
    "src/UI/*.cpp" 
    "src/UI/*.hpp"
)
file(GLOB_RECURSE SHADER_SOURCES "src/Assets/Shaders/*.glsl")

set(IMGUI_SOURCES
    external/imgui/imgui.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/backends/imgui_impl_glfw.cpp
    external/imgui/backends/imgui_impl_opengl3.cpp
)

set(IMGUI_FILEDIAL_SOURCES
    external/ImGuiFileDialog/ImGuiFileDialog.cpp
)

# Fix for stb_image duplicate definitions
add_definitions(
    -DTW_STATIC
    -DTW_NO_LIB_PRAGMA
    -DTW_NO_DIRECT3D
    -DGLEW_STATIC
    -D_CRT_SECURE_NO_WARNINGS
    -DGLFW_INCLUDE_NONE  # Prevents GLFW from including OpenGL headers
)

# Set compile options
set(CMAKE_BUILD_TYPE Debug)
add_compile_options(-g)

# TP1
add_executable(TP
    ${COMMON_SOURCES}
    ${SRC_SOURCES}
    ${SHADER_SOURCES}
    ${IMGUI_SOURCES}
    ${IMGUI_FILEDIAL_SOURCES}
    ${STB_IMAGE_SOURCE}  # Add stb_image separately
)

target_link_libraries(TP
    ${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(TP PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/")
create_target_launcher(TP WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/")

# Créer le répertoire de destination si nécessaire
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

# Commande de copie modifiée
add_custom_command(
   TARGET TP POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/TP${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_BINARY_DIR}/"
)

# Copy shader files and other assets to build directory
add_custom_command(
   TARGET TP POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy_directory
   "${CMAKE_SOURCE_DIR}/src/Assets"
   "${CMAKE_CURRENT_BINARY_DIR}/Assets"
)

SOURCE_GROUP(common REGULAR_EXPRESSION ".*/common/.*" )
SOURCE_GROUP(shaders REGULAR_EXPRESSION ".*/.*shader$" )

if (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )
# Remove this duplicate command that was here before
# add_custom_command(
#    TARGET TP POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/TP${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_BINARY_DIR}/"
# )
elseif (${CMAKE_GENERATOR} MATCHES "Xcode" )
  # Add Xcode-specific commands here if needed
endif (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )