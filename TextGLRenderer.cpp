#include "TextGLRenderer.h"

TextGLRenderer::TextGLRenderer()
{
    vbos = new GLuint[1];
    textures = new GLuint[1];
    uniformLocations = new GLuint[6];
    points = new float[12];
    points[0] = -1.0f;
    points[1] = -1.0f;
    points[2] = 1.0f;
    points[3] = -1.0f;
    points[4] = -1.0f;
    points[5] = 1.0f;
    points[6] = -1.0f;
    points[7] = 1.0f;
    points[8] = 1.0f;
    points[9] = -1.0f;
    points[10] = 1.0f;
    points[11] = 1.0f;
    numberOfVertices = 6;
    addPoints(0,points,6);
    generateVao();
}

TextGLRenderer::~TextGLRenderer()
{
    delete[] vbos;
    delete[] textures;
    delete[] uniformLocations;
    delete[] points;
}

void TextGLRenderer::draw()
{
    glUseProgram(shader);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glUniformMatrix4fv(uniformLocations[0], 1, GL_FALSE, modelMatrix);
    glUniform2fv(uniformLocations[1], 1, topLeft);
    glUniform2fv(uniformLocations[2], 1, bottomRight);
    glUniform2fv(uniformLocations[3], 1, textSize);
    glUniform3fv(uniformLocations[4], 1, color);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void TextGLRenderer::addShader(GLuint inShader)
{
    shader = inShader;
    glUseProgram(shader);
    
    uniformLocations[0] = glGetUniformLocation(shader,"model");
    uniformLocations[1] = glGetUniformLocation(shader,"texture_top_left");
    uniformLocations[2] = glGetUniformLocation(shader,"texture_bottom_right");
    uniformLocations[3] = glGetUniformLocation(shader,"inSize");
    uniformLocations[4] = glGetUniformLocation(shader,"inColor");
    uniformLocations[5] = glGetUniformLocation(shader,"text_texture");
    glUniform1i(uniformLocations[5],0);
}


void TextGLRenderer::setTextTexture(const unsigned int inWidth, const unsigned int inHeight, unsigned char* inData)
{
    glGenTextures(1,&textures[0]);
    glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, inWidth, inHeight, 0, GL_BGR,GL_UNSIGNED_BYTE, inData);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

}

void TextGLRenderer::setTextTexture(const GLuint inTextTexture)
{
    textures[0] = inTextTexture;
}

GLuint TextGLRenderer::getTextTexure()const
{
    return textures[0];
}

void TextGLRenderer::generateVao()
{
    glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
}

void TextGLRenderer::addPoints(const unsigned int inType, GLfloat* inPoints, const unsigned int inPointCount)
{
	glGenBuffers(1, &vbos[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	glBufferData(GL_ARRAY_BUFFER, inPointCount * 2 * sizeof(GLfloat),inPoints,GL_STATIC_DRAW);  

}
