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


if (WIN32)
	set(SDL2_INCLUDE_DIR lib/SDL2-2.0.4/include)
	set(SDL2_LIBRARY lib/SDL2-2.0.4/lib/x86/SDL2)
else()
	# includes cmake/FindSDL2.cmake
	set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules)
	find_package(SDL2 REQUIRED)
endif()

include_directories(${SDL2_INCLUDE_DIR})

include_directories(lib/glm)


add_executable(Tutorial src/main.cpp)
target_link_libraries(Tutorial ${SDL2_LIBRARY} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES})
