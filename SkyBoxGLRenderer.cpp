#include "SkyBoxGLRenderer.h"

SkyBoxGLRenderer::SkyBoxGLRenderer()
{
	textures = new GLuint[1];
	vbos = new GLuint[1];
	uniformLocations = new GLuint[3];
	cameraRotation = new float[16];
	projectionMatrix = new float[16];
	vaoStatus = 0x00;
	textureStatus = 0x00;
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &textures[0]);
}

SkyBoxGLRenderer::~SkyBoxGLRenderer()
{
	delete[] textures;
	delete[] vbos;
	delete[] uniformLocations;
	delete[] cameraRotation;
	delete[] projectionMatrix;
}
	
void SkyBoxGLRenderer::draw()
{
	glDepthMask(GL_FALSE);
	glUseProgram(shader);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textures[0]);
	glUniformMatrix4fv(uniformLocations[1],1,GL_FALSE,projectionMatrix);
	glUniform3fv(uniformLocations[0],1,cameraRotation);
	
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, numberOfVertices);
	glDepthMask(GL_TRUE);
}
		
void SkyBoxGLRenderer::addShader(GLuint inShader)
{
	shader = inShader;
	glUseProgram(shader);
	
	uniformLocations[0] = glGetUniformLocation(shader, "camera_rotation");
	uniformLocations[1] = glGetUniformLocation(shader, "proj_mat");
	uniformLocations[2] = glGetUniformLocation(shader, "cube_texture");
	glUniform1i(uniformLocations[2],0);
}

void SkyBoxGLRenderer::updateProjectionMatrix(const float* inProjectionMatrix)
{
		glUseProgram(shader);
		for(unsigned int i = 0; i < 16; i++)
		{
			projectionMatrix[i] = inProjectionMatrix[i];
		}
		glUniformMatrix4fv(uniformLocations[1],1,GL_FALSE,projectionMatrix);
}

void SkyBoxGLRenderer::updateCameraRotation(const float* inCameraRotation)
{
	glUseProgram(shader);
	for(unsigned int i = 0; i < 16; i++)
	{
		cameraRotation[i] = inCameraRotation[i];
	}
		glUniformMatrix4fv(uniformLocations[0],1,GL_FALSE,cameraRotation);
}

void SkyBoxGLRenderer::setVertices(GLfloat* inVertices,const unsigned int inNumberOfVertices)
{
	numberOfVertices = inNumberOfVertices;
	glGenBuffers(1, &vbos[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	glBufferData(GL_ARRAY_BUFFER, inNumberOfVertices * 3 * sizeof(GLfloat), inVertices, GL_STATIC_DRAW);  
	vaoStatus = vaoStatus | 0x01;
	if(vaoStatus == 0x01)
	{
		generateVao();
	}
}

void SkyBoxGLRenderer::setBoxTextureUp(const unsigned int inWidth, const unsigned int inHeight, unsigned char* inData)
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, textures[0]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, inWidth, inHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, inData);
	textureStatus = textureStatus | 0x01;
	if(textureStatus == 0x3F)
	{
		formatTextures();
	}
}

void SkyBoxGLRenderer::setBoxTextureDown(const unsigned int inWidth, const unsigned int inHeight, unsigned char* inData)
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, textures[0]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, inWidth, inHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, inData);
	textureStatus = textureStatus | 0x02;
	if(textureStatus == 0x3F)
	{
		formatTextures();
	}
}

void SkyBoxGLRenderer::setBoxTextureLeft(const unsigned int inWidth, const unsigned int inHeight, unsigned char* inData)
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, textures[0]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, inWidth, inHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, inData);
	textureStatus = textureStatus | 0x04;
	if(textureStatus == 0x3F)
	{
		formatTextures();
	}
}

void SkyBoxGLRenderer::setBoxTextureRight(const unsigned int inWidth, const unsigned int inHeight, unsigned char* inData)
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, textures[0]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, inWidth, inHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, inData);
	textureStatus = textureStatus | 0x08;
	if(textureStatus == 0x3F)
	{
		formatTextures();
	}
}

void SkyBoxGLRenderer::setBoxTextureFront(const unsigned int inWidth, const unsigned int inHeight, unsigned char* inData)
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, textures[0]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, inWidth, inHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, inData);
	textureStatus = textureStatus | 0x10;
	if(textureStatus == 0x3F)
	{
		formatTextures();
	}
}

void SkyBoxGLRenderer::setBoxTextureBack(const unsigned int inWidth, const unsigned int inHeight, unsigned char* inData)
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, textures[0]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, inWidth, inHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, inData);
	textureStatus = textureStatus | 0x20;
	if(textureStatus == 0x3F)
	{
		formatTextures();
	}
}

void SkyBoxGLRenderer::generateVao()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	
	glEnableVertexAttribArray(0);
}
void SkyBoxGLRenderer::formatTextures()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, textures[0]);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}
