cmake_minimum_required(VERSION 3.10)
project(Universe)

# Set Clang as the compiler
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

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
include_directories("${CMAKE_SOURCE_DIR}/lib/imgui")

# Add source files
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.hpp" "include/*/*.hpp")

# Add Glad header files
file(GLOB_RECURSE GLAD_HEADERS "${CMAKE_SOURCE_DIR}/lib/glad/*.c" "${CMAKE_SOURCE_DIR}/lib/glad/*.h")

# Find OpenGL
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

# Find GLFW
find_package(glfw3 3.3 REQUIRED)

# Find OpenMP
find_package(OpenMP REQUIRED)

# Add ImGui source files
file(GLOB_RECURSE IMGUI_SOURCES "${CMAKE_SOURCE_DIR}/lib/imgui/*.cpp")

# Add executable
add_executable(universe ${SOURCES} ${HEADERS} ${IMGUI_SOURCES} ${GLAD_HEADERS})

# Link libraries (OpenGL, GLFW, OpenMP, etc.)
target_link_libraries(universe OpenGL::GL)

if (WIN32)
    target_link_libraries(universe winmm.lib)
    if (MSVC)
        target_link_libraries(universe "${CMAKE_SOURCE_DIR}/lib/glfw/lib-vc2019/glfw3.lib")
    elseif (MINGW)
        target_link_libraries(universe "${CMAKE_SOURCE_DIR}/lib/glfw/lib-mingw/glfw3.lib")
    endif()
elseif (UNIX)
    target_link_libraries(universe glfw ${CMAKE_DL_LIBS})
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

# Set the working directory for VS debugger
set_property(TARGET universe PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
