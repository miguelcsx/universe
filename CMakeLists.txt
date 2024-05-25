cmake_minimum_required(VERSION 3.10)
project(Universe)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)

# Add include directories
include_directories("${CMAKE_SOURCE_DIR}/include")
include_directories("${CMAKE_SOURCE_DIR}/lib")
include_directories("${CMAKE_SOURCE_DIR}/lib/glad")
include_directories("${CMAKE_SOURCE_DIR}/lib/glad/include")
include_directories("${CMAKE_SOURCE_DIR}/lib/glfw")
include_directories("${CMAKE_SOURCE_DIR}/lib/glfw/include")
include_directories("${CMAKE_SOURCE_DIR}/lib/imgui")  # Add ImGui include directory

# Add source files
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.hpp" "include/*/*.hpp")

# Add ImGui header files
file(GLOB_RECURSE IMGUI_HEADERS "${CMAKE_SOURCE_DIR}/lib/imgui/*.cpp" "${CMAKE_SOURCE_DIR}/lib/imgui/*.h")

# Add Glad header files
file(GLOB_RECURSE GLAD_HEADERS "${CMAKE_SOURCE_DIR}/lib/glad/*.c" "${CMAKE_SOURCE_DIR}/lib/glad/*.h")

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL 4.3 REQUIRED)

# Add executable
add_executable(universe ${SOURCES} ${HEADERS} ${IMGUI_HEADERS} ${GLAD_HEADERS})

# Link libraries (OpenGL, GLFW, etc.) if needed
target_link_libraries(universe OpenGL::GL)

find_package(glfw3 3.3 REQUIRED)
target_link_libraries(universe glfw ${CMAKE_DL_LIBS})

set_property(TARGET universe PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
