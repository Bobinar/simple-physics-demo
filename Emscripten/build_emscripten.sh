mkdir Build
emcc ../src/main.cpp -o Build/main.html -s USE_GLFW=3 -I../lib/glm