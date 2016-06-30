#ifndef BACKGROUND_H
#define BACKGROUND_H
#include "drawable.h"

//4 boxes texture, rbrb
const GLubyte texture[] = {
    255,0,0,255,  255,0,0,255,  255,0,0,255,  255,0,0,255,  0,0,255,255,  0,0,255,255,  0,0,255,255,  0,0,255,255,
    255,0,0,255,  255,0,0,255,  255,0,0,255,  255,0,0,255,  0,0,255,255,  0,0,255,255,  0,0,255,255,  0,0,255,255,
    255,0,0,255,  255,0,0,255,  255,0,0,255,  255,0,0,255,  0,0,255,255,  0,0,255,255,  0,0,255,255,  0,0,255,255,
    255,0,0,255,  255,0,0,255,  255,0,0,255,  255,0,0,255,  0,0,255,255,  0,0,255,255,  0,0,255,255,  0,0,255,255,
    0,0,255,255,  0,0,255,255,  0,0,255,255,  0,0,255,255,  255,0,0,255,  255,0,0,255,  255,0,0,255,  255,0,0,255,
    0,0,255,255,  0,0,255,255,  0,0,255,255,  0,0,255,255,  255,0,0,255,  255,0,0,255,  255,0,0,255,  255,0,0,255,
    0,0,255,255,  0,0,255,255,  0,0,255,255,  0,0,255,255,  255,0,0,255,  255,0,0,255,  255,0,0,255,  255,0,0,255,
    0,0,255,255,  0,0,255,255,  0,0,255,255,  0,0,255,255,  255,0,0,255,  255,0,0,255,  255,0,0,255,  255,0,0,255
};

//two triangles put together to make a square
const float positions[] = {
    -1, -1, 0,
    -1, 1, 0,
    1, -1, 0,
    1, -1, 0,
    -1, 1, 0,
    1, 1, 0
};
const int texUnit=0;

class BackgroundTile{

public:
    int width;
    int height;
    GLubyte* img;
    BackgroundTile(int width, int height, int color);
    ~BackgroundTile(){
        delete[] img;
    }
};

class Background
{
    std::vector<BackgroundTile*> textures;
    GLuint vao;
    GLuint positionBufferObject;
    GLuint texBufferdObject;
    GLuint theProgram;
public:
    Background();
    void bufferData(GLuint &program);
    void draw();
    void generateTiles();
    ~Background(){
        for(BackgroundTile* tile: textures){
            delete tile;
        }
    }
};

#endif // BACKGROUND_H
