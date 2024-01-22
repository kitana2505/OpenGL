#include <iostream>
#include <stdlib.h> 
#include <queue>
#include "Skybox.h"



void Skybox::update(float elapsedTime, const glm::mat4* parentModelMatrix) {
	return;
}
void Skybox::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
	if (initialized) {
		glUseProgram(shaderProgram->program);
		for (auto geometry : geometries) {
			// compose transformations
			glm::mat4 matrix = projectionMatrix * viewMatrix;

			glm::mat4 viewRotation = viewMatrix;
			viewRotation[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

			glm::mat4 inversePVmatrix = glm::inverse(projectionMatrix * viewRotation);
			glUniformMatrix4fv(shaderProgram->iPVM, 1, GL_FALSE, glm::value_ptr(inversePVmatrix));
			glUniform1i(shaderProgram->Sampler, 0);

			glBindVertexArray(geometry->vertexArrayObject);
			glBindTexture(GL_TEXTURE_CUBE_MAP, geometry->texture);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, geometry->numTriangles + 2);

			glBindVertexArray(0);
		}

		glUseProgram(0);
	}
}

Skybox::Skybox(SkyboxShaderProgram* shdrPrg)
{
	initialized = true;
	this->shaderProgram = shdrPrg;
	geometries.push_back(new ObjectGeometry);
	load_skybox(SKYBOX_NIGHT_TEXTURE_NAME, night_suffixes);
	
}

void Skybox::load_skybox(const char* prefix, const char* suffixes[6]) {
	static const float screenCoords[] = {

		1.0f, 1.0f,
	   -1.0f, 1.0f,
	  1.0f,  -1.0f,
	   -1.0f,  -1.0f
	};

	glGenVertexArrays(1, &((geometries[0])->vertexArrayObject));
	glBindVertexArray((geometries[0])->vertexArrayObject);

	// buffer for far plane rendering
	glGenBuffers(1, &((geometries[0])->vertexBufferObject)); \
	glBindBuffer(GL_ARRAY_BUFFER, (geometries[0])->vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screenCoords), screenCoords, GL_STATIC_DRAW);

	//glUseProgram(farplaneShaderProgram);

	glEnableVertexAttribArray(shaderProgram->screenCoord);
	glVertexAttribPointer(shaderProgram->screenCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
	glUseProgram(0);
	CHECK_GL_ERROR();

	(geometries[0])->numTriangles = 2;

	glActiveTexture(GL_TEXTURE0);

	glGenTextures(1, &((geometries[0])->texture));
	glBindTexture(GL_TEXTURE_CUBE_MAP, (geometries[0])->texture);

	GLuint targets[] = {
	  GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	  GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	  GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};
	CHECK_GL_ERROR();
	for (int i = 0; i < 6; i++) {
		std::string texName = std::string(prefix) + "_" + suffixes[i] + ".jpg";
		CHECK_GL_ERROR();
		std::cout << "Loading cube map texture: " << texName << std::endl;
		if (!pgr::loadTexImage2D(texName, targets[i])) {
			pgr::dieWithError("Skybox cube map loading failed!");
		}
		CHECK_GL_ERROR();
	}
	CHECK_GL_ERROR();
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);


	// unbind the texture (just in case someone will mess up with texture calls later)
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	CHECK_GL_ERROR();
}

Skybox::~Skybox() {
	for (auto geometry : geometries) {
		glDeleteVertexArrays(1, &(geometry->vertexArrayObject));
		glDeleteBuffers(1, &(geometry->elementBufferObject));
		glDeleteBuffers(1, &(geometry->vertexBufferObject));
		delete geometry;
	}

	initialized = false;
}
