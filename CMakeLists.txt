cmake_minimum_required (VERSION 2.6)
project (Tutorial)
include_directories(src)

if (WIN32)
	set(CMAKE_PREFIX_PATH "lib/glew-1.13.0/include")
	set(CMAKE_LIBRARY_PATH "lib/glew-1.13.0/lib/Release/Win32/")
endif()

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

find_package(OpenGL REQUIRED)
include_directories(${GL_INCLUDE_DIRS})

add_subdirectory(glfw-3.3)
include_directories(glfw-3.3/include)

include_directories(lib/glm)

add_executable(Tutorial src/main.cpp)
target_link_libraries(Tutorial glfw ${GLFW_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES})
