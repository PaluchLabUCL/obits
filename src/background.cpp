#include "background.h"
Background::Background()
{

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
    glBindTexture(GL_TEXTURE_2D, texBufferdObject);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, &texture[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);


    GLuint samplerUniform = glGetUniformLocation(program, "texSampler");
    glUniform1i(samplerUniform, texUnit);
    glUseProgram(0);

}

void Background::draw(){
    glUseProgram(theProgram);
    glBindVertexArray(vao);

    glActiveTexture(GL_TEXTURE0+texUnit);
    glBindTexture(GL_TEXTURE_2D, texBufferdObject);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
}
