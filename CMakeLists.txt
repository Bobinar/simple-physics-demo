cmake_minimum_required (VERSION 3.1)
project (Tutorial)
include_directories(include)

set (CMAKE_CXX_STANDARD 14)

include_directories(lib/glm)

add_executable(Tutorial src/main.cpp src/QuadMesh.cpp src/SceneInitialization.cpp src/Frustum.cpp src/ShaderFactory.cpp src/SphereMesh.cpp src/SceneRenderer.cpp src/SceneSimulator.cpp src/SceneManager.cpp src/Sphere.cpp)

find_package(OpenGL REQUIRED)
include_directories(${GL_INCLUDE_DIRS})

if(CMAKE_CXX_COMPILER MATCHES "/em\\+\\+(-[a-zA-Z0-9.])?$")

  	# Emscripten 
  	SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -s USE_SDL=2")
	target_link_libraries(Tutorial "-s USE_SDL=2")
else()
  	# Rest of platforms 
	if (WIN32)
		set(CMAKE_PREFIX_PATH "lib/glew-1.13.0/include")
		set(CMAKE_LIBRARY_PATH "lib/glew-1.13.0/lib/Release/x64/")
		file(COPY ${CMAKE_CURRENT_LIST_DIR}/lib/glew-1.13.0/bin/Release/x64/glew32.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
	endif()

	find_package(GLEW REQUIRED)
	include_directories(${GLEW_INCLUDE_DIRS})

	if (WIN32)
		set(SDL2_INCLUDE_DIR lib/SDL2-2.0.10/include)
		set(SDL2_LIBRARY ${CMAKE_CURRENT_LIST_DIR}/lib/SDL2-2.0.10/lib/x64/SDL2.lib)
		file(COPY ${CMAKE_CURRENT_LIST_DIR}/lib/SDL2-2.0.10-win32-x64/SDL2.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
	else()
		# includes cmake/FindSDL2.cmake
		set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules)
		find_package(SDL2 REQUIRED)
	endif()
	
	include_directories(${SDL2_INCLUDE_DIR})	

	target_link_libraries(Tutorial ${SDL2_LIBRARY} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES})

endif()



