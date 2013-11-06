
GLFWDIR="/Users/msmith/Documents/development/c/orbits"
cd src
g++ -std=gnu++11 -I"../include" -pthread \
-lglfw -lGLU -lGL -lGLEW -lX11 -lXrandr -ldl -lm \
main.cpp Framework.cpp plot.cpp -o ../build/plot_window
cd ..
cp shaders/* build/
