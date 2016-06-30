
GLFWDIR="/Users/msmith/.local"
cd src
g++ -I"$GLFWDIR/include" -L"$GLFWDIR"/lib -framework carbon -framework iokit -framework cocoa -framework opengl -framework corevideo -lglfw3 main.cpp Framework.cpp plot.cpp world.cpp character.cpp plotwindow.cpp background.cpp dynamicobject.cpp plotcamera.cpp plotinteractor.cpp -o ../build/plot_window
cd ..
cp shaders/* build/
