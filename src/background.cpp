#include "background.h"
#include <math.h>
Background::Background()
{
    generateTiles();
}

void Background::bufferData(GLuint &program){
    /*
     * Set up the buffers.
    */
    theProgram = program;
    glUseProgram(theProgram);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &positionBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*6*3,positions, GL_STREAM_DRAW);

    GLuint posIndex = glGetAttribLocation(program, "pos");
    glEnableVertexAttribArray(posIndex);
    glVertexAttribPointer(posIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
    glUseProgram(0);


    /*
     * Set up texture buffer
     **/
    glUseProgram(theProgram);

    glGenTextures(1, &texBufferdObject);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texBufferdObject);
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, 2*textures[0]->width, 2*textures[0]->height, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL  );

    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, textures[0]->width, textures[0]->height, 1, GL_RGBA, GL_UNSIGNED_BYTE, textures[0]->img);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, textures[0]->width, 0, 0, textures[0]->width, textures[0]->height, 1, GL_RGBA, GL_UNSIGNED_BYTE, textures[1]->img);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, textures[0]->width, textures[0]->height, 0, textures[0]->width, textures[0]->height, 1, GL_RGBA, GL_UNSIGNED_BYTE, textures[2]->img);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, textures[0]->height, 0, textures[0]->width, textures[0]->height, 1, GL_RGBA, GL_UNSIGNED_BYTE, textures[3]->img);

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);



    GLuint samplerUniform = glGetUniformLocation(program, "texSampler");
    glUniform1i(samplerUniform, texUnit);
    glUseProgram(0);

}

void Background::generateTiles(){
    //same thing we have now
    for(int i = 0; i<4; i++){
        int color = 0xff000000 + (0xff<<(8*(i%3))) + (i/3)*(0xffff<<8);
        textures.push_back(new BackgroundTile(16, 16, color) );
    }
}

void Background::draw(){
    glUseProgram(theProgram);
    glBindVertexArray(vao);

    glActiveTexture(GL_TEXTURE0+texUnit);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texBufferdObject);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    glUseProgram(0);
}

BackgroundTile::BackgroundTile(int width, int height, int color){
    this->width = width;
    this->height = height;
    GLubyte r = 0xff&(color>>16);
    GLubyte g = 0xff&(color>>8);
    GLubyte b = 0xff&(color);
    GLubyte a = 0xff&(color>>24);

    img = new GLubyte[4*width*height];
    int cx = width/2;
    int cy = height/2;
    float factor;
    for(int i = 0; i<width*height; i++){
        int dx = i/width - cx;
        int dy = i%width - cy;
        float r = dx*dx + dy*dy;

        if(r>0){

            factor = 1.0/pow(r, 0.75);
        } else{
            factor = 1;
        }
        GLubyte* o = &img[4*i];
        o[3] = GLubyte(r*factor);
        o[1] = GLubyte(g*factor);;
        o[2] = GLubyte(b*factor);;
        o[0] = GLubyte(a*factor);
    }
}
