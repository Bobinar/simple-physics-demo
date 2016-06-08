mkdir Build
emcc ../src/main.cpp -o Build/main.html -s USE_SDL=2 -I..\lib\glm