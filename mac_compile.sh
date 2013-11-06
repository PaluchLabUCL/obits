
GLFWDIR="/Users/msmith/Documents/development/c/orbits"
cd src
g++ -I"$GLFWDIR/include" -L"$GLFWDIR"/lib -framework iokit -framework cocoa -framework opengl -lglfw main.cpp Framework.cpp plot.cpp -o ../build/plot_window
cd ..
cp shaders/* build/
