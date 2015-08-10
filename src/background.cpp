#include "background.h"
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
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, 8, 8, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL  );

    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, 4, 4, 1, GL_RGBA, GL_UNSIGNED_BYTE, textures[0]->img);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 4, 0, 0, 4, 4, 1, GL_RGBA, GL_UNSIGNED_BYTE, textures[1]->img);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 4, 4, 0, 4, 4, 1, GL_RGBA, GL_UNSIGNED_BYTE, textures[2]->img);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 4, 0, 4, 4, 1, GL_RGBA, GL_UNSIGNED_BYTE, textures[3]->img);

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
        textures.push_back(new BackgroundTile(4, 4, 0xff000000^(0x55<<(8*(i%3)))) );
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
    printf("%d, %d, %d, %d\n", r, g, b, a);
    img = new GLubyte[4*width*height];
    for(int i = 0; i<width*height; i++){
        GLubyte* o = &img[4*i];
        o[0] = r;
        o[1] = g;
        o[2] = b;
        o[3] = a;
    }
}
