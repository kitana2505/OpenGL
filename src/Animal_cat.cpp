#include <iostream>
#include "Animal_cat.h"


void Cat::update(float elapsedTime, const glm::mat4* parentModelMatrix) {
	ObjectInstance::update(elapsedTime, parentModelMatrix);
}

void Cat::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
	if (initialized && (shaderProgram != nullptr)) {
		glUseProgram(shaderProgram->program);
		for (auto geometry : geometries) {
			for (auto location : locations) {
				geometry->texture = pgr::createTexture(CAT_TEXTURE);

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
				glBindTexture(GL_TEXTURE_2D, geometry->texture);
				glDrawElements(GL_TRIANGLES, geometry->numTriangles * 3, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
				glBindTexture(GL_TEXTURE_2D, 0);


			}
		}
		glUseProgram(0);

	}
	else {
		std::cout << "Can't draw Cat: triangle not initialized properly!" << std::endl;
	}
}

Cat::Cat(ShaderProgram* shdrPrg) : ObjectInstance(shdrPrg), initialized(false)
{
	if (loadMultipleMeshes(CAT_MODEL, shdrPrg, geometries) != true) {
		std::cerr << "initializeModels(): Cat model loading failed." << std::endl;
	}
	else {
		//geometries[0]->texture= pgr::createTexture("\data\Spruce_obj");;
		initialized = true;
	}

	glm::mat4 location = glm::scale(globalModelMatrix, glm::vec3(CAT_SCALE));
	location = glm::translate(location, CAT_INITIAL_POS);
	location = glm::rotate(location, CAT_ROTATION, glm::vec3(0, 1, 0));
	locations.emplace_back(location);

	this->position = CAT_INITIAL_POS;
	this->direction = -CAT_INITIAL_POS;

	CHECK_GL_ERROR();
	initialized = true;
}
Cat::~Cat() {
	for (auto geometry : geometries) {
		glDeleteVertexArrays(1, &(geometry->vertexArrayObject));
		glDeleteBuffers(1, &(geometry->elementBufferObject));
		glDeleteBuffers(1, &(geometry->vertexBufferObject));
		delete geometry;
	}

	initialized = false;
}