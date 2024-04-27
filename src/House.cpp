#include <iostream>
#include "House.h"


void House::update(float elapsedTime, const glm::mat4* parentModelMatrix) {
	ObjectInstance::update(elapsedTime, parentModelMatrix);
}

void House::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
	if (initialized && (shaderProgram != nullptr)) {
		glUseProgram(shaderProgram->program);
		for (auto geometry : geometries) {
			for (auto location : locations) {
				geometry->texture = pgr::createTexture(HOUSE_TEXTURE);

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
				glActiveTexture(GL_TEXTURE0); // select texture unit 0
				glBindTexture(GL_TEXTURE_2D,geometry->texture);
				glDrawElements(GL_TRIANGLES, geometry->numTriangles * 3, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
				glBindTexture(GL_TEXTURE_2D, 0);

			}
		}
		glUseProgram(0);

	}
	else {
		std::cout << "Can't draw House: triangle not initialized properly!" << std::endl;
	}
}

House::House(ShaderProgram* shdrPrg) : ObjectInstance(shdrPrg), initialized(false)
{
	//this->shaderProgram = shdrPrg;
	if (loadMultipleMeshes(HOUSE_MODEL, shdrPrg, geometries) != true) {
		std::cerr << "initializeModels(): House model loading failed." << std::endl;
	}
	else {
		initialized = true;
	}
	direction = glm::vec3(0.0f, 1.0f, 0.0f);
	position = HOUSE_TRANSLATE;
	glm::mat4 location = glm::scale(globalModelMatrix, glm::vec3(HOUSE_SCALE));
	location = glm::translate(location, position);
	locations.emplace_back(location);

	CHECK_GL_ERROR();

}
House::~House() {
	for (auto geometry : geometries) {
		glDeleteVertexArrays(1, &(geometry->vertexArrayObject));
		glDeleteBuffers(1, &(geometry->elementBufferObject));
		glDeleteBuffers(1, &(geometry->vertexBufferObject));
		delete geometry;
	}

	initialized = false;
}
