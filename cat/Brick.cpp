#include <iostream>
#include "Brick.h"
BrickShaderProgram* brickShdr;

void Brick::update(float elapsedTime, const glm::mat4* parentModelMatrix) {
	ObjectInstance::update(elapsedTime, parentModelMatrix);
}

void Brick::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
	if (initialized && (shaderProgram != nullptr)) {
		glUseProgram(shaderProgram->program);
		for (auto geometry : geometries) {
			position = BRICK_INITIAL_POS;
			glm::mat4 location = glm::scale(globalModelMatrix, glm::vec3(BRICK_SCALE));
			location = glm::translate(location, glm::vec3(position));
			location = glm::rotate(location, BRICK_ROTATION, glm::vec3(1, 0, 0));
				glBindVertexArray(geometry->vertexArrayObject);
				setTransformUniforms(*shaderProgram, location * localModelMatrix, viewMatrix, projectionMatrix);
				
				setMaterialUniforms(
					*shaderProgram,
					geometry->ambient,
					geometry->diffuse,
					geometry->specular,
					geometry->shininess,
					geometry->texture
				);
				glActiveTexture(GL_TEXTURE1); // select texture unit 1
				glBindTexture(GL_TEXTURE_2D, mossTex);
				glUniform1i(shaderProgram->locations.mossTex, 1);


				glDrawElements(GL_TRIANGLES, geometry->numTriangles * 3, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);


			//}
		}
		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);

	}
	else {
		std::cout << "Can't draw Brick: triangle not initialized properly!" << std::endl;
	}
}

Brick::Brick(ShaderProgram* shdrPrg) : ObjectInstance(shdrPrg),  initialized(false)
{
	//this->shaderProgram = shdrPrg;
	if (loadMultipleMeshes(BRICK_MODEL, shdrPrg, geometries) != true) {
		std::cerr << "initializeModels():brick model loading failed." << std::endl;
	}
	else {
		initialized = true;
	}
	
	mossTex = pgr::createTexture("data/brick/moss.png");
	glActiveTexture(GL_TEXTURE1); // select texture unit 1
	glBindTexture(GL_TEXTURE_2D, mossTex);
	
	glUniform1i(shdrPrg->locations.mossTex, 1);


	CHECK_GL_ERROR();
	initialized = true;
}
Brick::~Brick() {
	for (auto geometry : geometries) {
		glDeleteVertexArrays(1, &(geometry->vertexArrayObject));
		glDeleteBuffers(1, &(geometry->elementBufferObject));
		glDeleteBuffers(1, &(geometry->vertexBufferObject));
		delete geometry;
	}

	initialized = false;
}
