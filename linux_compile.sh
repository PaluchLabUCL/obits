
GLFWDIR="/Users/msmith/Documents/development/c/orbits"

export GLFW_PC="$GLFW3_HOME"/lib/pkgconfig/glfw3.pc
export GLFW_CFLAGS=$(pkg-config --cflags "$GLFW_PC")
export GLFW_LIBS=$(pkg-config --static --libs "$GLFW_PC")

g++ -std=c++11 $GLFW_CFLAGS src/*.cpp -lGLEW $GLFW_LIBS -o build/orbits

cd ..
