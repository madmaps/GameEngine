#include "BumpMapGLRenderer.h"

	BumpMapGLRenderer::BumpMapGLRenderer()
	{
		vbos = new GLuint[4];
		textures = new GLuint[4];
		uniformLocations = new GLuint[8];
		projectionMatrix = new float[16];
		viewMatrix = new float[16];
		modelMatrix = new float[16];
		cameraLocation = new float[3];
		vaoStatus = 0x00;
	}
	
	BumpMapGLRenderer::~BumpMapGLRenderer()
	{
		delete[] vbos;
		delete[] textures;
		delete[] uniformLocations;
		delete[] projectionMatrix;
		delete[] viewMatrix;
		delete[] modelMatrix;
		delete[] cameraLocation;
	}

	void BumpMapGLRenderer::draw()
	{
		glUseProgram(shader);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textures[1]);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, textures[2]);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, textures[3]);
		glUniformMatrix4fv(uniformLocations[1],1,GL_FALSE,projectionMatrix);
		glUniformMatrix4fv(uniformLocations[0],1,GL_FALSE,viewMatrix);
		glUniformMatrix4fv(uniformLocations[2],1,GL_FALSE,modelMatrix);
		glUniform3fv(uniformLocations[3],1,cameraLocation);

		
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, numberOfVertices);

	}
	
	void BumpMapGLRenderer::generateVao()
	{
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, vbos[3]);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, NULL);
		
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
	}
	
	void BumpMapGLRenderer::addShader(GLuint inShader)
	{
		shader = inShader;
		glUseProgram(shader);
		
		uniformLocations[0] = glGetUniformLocation(shader,"view");
		uniformLocations[1] = glGetUniformLocation(shader,"proj");
		uniformLocations[2] = glGetUniformLocation(shader,"model");
		uniformLocations[3] = glGetUniformLocation(shader,"cameraPosition_world");
		uniformLocations[4] = glGetUniformLocation(shader,"basic_texture");
		uniformLocations[5] = glGetUniformLocation(shader,"normal_map");
		uniformLocations[6] = glGetUniformLocation(shader,"specular_map");
		uniformLocations[7] = glGetUniformLocation(shader,"occlusion_map");
		glUniform1i(uniformLocations[4],0);
		glUniform1i(uniformLocations[5],1);
		glUniform1i(uniformLocations[6],2);
		glUniform1i(uniformLocations[7],3);
	}
	
	void BumpMapGLRenderer::updateProjectionMatrix(const float* inProjectionMatrix)
	{
		glUseProgram(shader);
		for(unsigned int i = 0; i < 16; i++)
		{
			projectionMatrix[i] = inProjectionMatrix[i];
		}
		glUniformMatrix4fv(uniformLocations[1],1,GL_FALSE,inProjectionMatrix);
	}
	
	void BumpMapGLRenderer::updateViewMatrix(const float* inViewMatrix)
	{
		glUseProgram(shader);
		for(unsigned int i = 0; i < 16; i++)
		{
			viewMatrix[i] = inViewMatrix[i];
		}
		glUniformMatrix4fv(uniformLocations[0],1,GL_FALSE,inViewMatrix);
	}
		
	void BumpMapGLRenderer::updateModelMatrix(const float* inModelMatrix)
	{
		glUseProgram(shader);
		for(unsigned int i = 0; i < 16; i++)
		{
			modelMatrix[i] = inModelMatrix[i];
		}
		glUniformMatrix4fv(uniformLocations[2],1,GL_FALSE,inModelMatrix);
	}
	
	void BumpMapGLRenderer::updateCameraLocation(const float* inCameraLocation)
	{
		glUseProgram(shader);
		for(unsigned int i = 0; i < 3; i++)
		{
			cameraLocation[i] = inCameraLocation[i];
		}
		glUniform3fv(uniformLocations[3],1,inCameraLocation);
	}
	
	void BumpMapGLRenderer::setVertices(GLfloat* inVertices,const unsigned int numberOfPoints)
	{
		numberOfVertices = numberOfPoints;
		addPoints(0,inVertices,numberOfPoints);
		vaoStatus = vaoStatus | 0x01;
		if(vaoStatus == 0x0F)
		{
			generateVao();
		}
	}
	
	void BumpMapGLRenderer::setNormals(GLfloat* inNormals, const unsigned int numberOfPoints)
	{
		addPoints(1,inNormals,numberOfPoints);
		vaoStatus = vaoStatus | 0x02;
		if(vaoStatus == 0x0F)
		{
			generateVao();
		}
	}
	
	void BumpMapGLRenderer::setTextureCoordinates(GLfloat* inTextureCoordinates,const unsigned int numberOfPoints)
	{
		addPoints(2,inTextureCoordinates,numberOfPoints);
		vaoStatus = vaoStatus | 0x04;
		if(vaoStatus == 0x0F)
		{
			generateVao();
		}
	}
	
	void BumpMapGLRenderer::setTangents(GLfloat* inTangents,const unsigned int numberOfPoints)
	{
		addPoints(3,inTangents,numberOfPoints);
		vaoStatus = vaoStatus | 0x08;
		if(vaoStatus == 0x0F)
		{
			generateVao();
		}
	}
	
	void BumpMapGLRenderer::setDiffuseTexture(const unsigned int inWidth,const unsigned int inHeight, unsigned char* inData)
	{
		addTexture(0, inWidth, inHeight, inData);
	}
	
	void BumpMapGLRenderer::setNormalTexture(const unsigned int inWidth,const unsigned int inHeight, unsigned char* inData)
	{
		addTexture(1, inWidth, inHeight, inData);
	}

	void BumpMapGLRenderer::setSpecularTexture(const unsigned int inWidth,const unsigned int inHeight, unsigned char* inData)
	{
		addTexture(2, inWidth, inHeight, inData);
	}

	void BumpMapGLRenderer::setOcclusionTexture(const unsigned int inWidth,const unsigned int inHeight, unsigned char* inData)
	{
		addTexture(3, inWidth, inHeight, inData);
	}


	void BumpMapGLRenderer::addPoints(const unsigned int inType, GLfloat* inPoints, const unsigned int inPointCount)
	{
		int numberPerType = 0;
		switch(inType)
		{
			case 0:
			case 1:
				numberPerType = 3;
				break;
			case 2:
				numberPerType = 2;
				break;
			case 3:
				numberPerType = 4;
				break;
		};
		
		glGenBuffers(1, &vbos[inType]);
		glBindBuffer(GL_ARRAY_BUFFER, vbos[inType]);
		glBufferData(GL_ARRAY_BUFFER, inPointCount * numberPerType * sizeof(GLfloat),inPoints,GL_STATIC_DRAW);  
	}  
	
	void BumpMapGLRenderer::addTexture(const unsigned int inType, const unsigned int inWidth, const unsigned int inHeight, unsigned char* inData)
	{
		glGenTextures(1,&textures[inType]);
		switch(inType)
		{
			case 0:
				glActiveTexture(GL_TEXTURE0);
				break;
			case 1:
				glActiveTexture(GL_TEXTURE1);
				break;
			case 2:
				glActiveTexture(GL_TEXTURE2);
				break;
			case 3:
				glActiveTexture(GL_TEXTURE3);
				break;
		};
		glBindTexture(GL_TEXTURE_2D, textures[inType]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, inWidth, inHeight, 0, GL_BGR,GL_UNSIGNED_BYTE, inData);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
		
