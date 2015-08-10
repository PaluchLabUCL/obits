
GLFWDIR="/Users/msmith/Documents/development/c/glfw"
cd src
g++ -I"$GLFWDIR/include" -L"$GLFWDIR"/lib -framework iokit -framework cocoa -framework opengl -framework corevideo -lglfw3 main.cpp Framework.cpp plot.cpp world.cpp character.cpp -o ../build/plot_window
cd ..
cp shaders/* build/
