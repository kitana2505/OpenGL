#include <iostream>
#include "Animal_rabbit.h"


void Rabbit::update(float elapsedTime, const glm::mat4* parentModelMatrix) {
	ObjectInstance::update(elapsedTime, parentModelMatrix);
}

void Rabbit::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
	if (initialized && (shaderProgram != nullptr)) {
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glUseProgram(shaderProgram->program);
		for (auto geometry : geometries) {
			for (auto location : locations) {
				//TODO
				//glStencilFunc(GL_ALWAYS, i + 2, 0xFF);
				localModelMatrix = glm::translate(glm::mat4(1.0f), position);
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
				glDrawElements(GL_TRIANGLES, geometry->numTriangles * 3, GL_UNSIGNED_INT, 0);
				//glDisable(GL_STENCIL_TEST);
				glBindVertexArray(0);


			}
		}
		glUseProgram(0);

	}
	else {
		//std::cout << "Can't draw Cat: triangle not initialized properly!" << std::endl;
	}
}

Rabbit::Rabbit(ShaderProgram* shdrPrg) : ObjectInstance(shdrPrg), initialized(false)
{
	//this->shaderProgram = shdrPrg;
	if (loadMultipleMeshes(RABBIT_MODEL, shdrPrg, geometries) != true) {
		std::cerr << "initializeModels(): Cat model loading failed." << std::endl;
	}
	else {
		//geometries[0]->texture= pgr::createTexture("\data\Spruce_obj");;
		initialized = true;
	}

	glm::mat4 location = glm::scale(globalModelMatrix, glm::vec3(RABBIT_SCALE));
	//location = glm::translate(location, Rabbit_INITIAL_POS);
	//location = glm::rotate(location, Rabbit_ROTATION, glm::vec3(0, 1, 0));
	locations.emplace_back(location);

	CHECK_GL_ERROR();
	destroyed = false;
	initialized = true;
}
Rabbit::~Rabbit() {
	for (auto geometry : geometries) {
		glDeleteVertexArrays(1, &(geometry->vertexArrayObject));
		glDeleteBuffers(1, &(geometry->elementBufferObject));
		glDeleteBuffers(1, &(geometry->vertexBufferObject));
		delete geometry;
	}

	initialized = false;
}
