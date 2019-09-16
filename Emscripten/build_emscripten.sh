mkdir Build
emcc ../src/main.cpp ../src/QuadMesh.cpp ../src/SceneInitialization.cpp ../src/Frustum.cpp ../src/ShaderFactory.cpp ../src/SphereMesh.cpp ../src/SceneRenderer.cpp ../src/SceneSimulator.cpp ../src/SceneManager.cpp ../src/Sphere.cpp -o Build/main.html -s USE_SDL=2 -I../lib/glm -I../include
