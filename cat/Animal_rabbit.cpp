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
				//glStencilFunc(GL_ALWAYS, i + 2, 0xFF);
				geometry->texture = pgr::createTexture(RABBIT_TEXTURE);
				CHECK_GL_ERROR();
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
				glActiveTexture(GL_TEXTURE0); // select texture unit 0
				glBindTexture(GL_TEXTURE_2D, geometry->texture);
				glDrawElements(GL_TRIANGLES, geometry->numTriangles * 3, GL_UNSIGNED_INT, 0);
				//glDisable(GL_STENCIL_TEST);
				glBindVertexArray(0);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}
		glUseProgram(0);

	}
	else {
		std::cout << "Can't draw Rabbit: triangle not initialized properly!" << std::endl;
	}
}

Rabbit::Rabbit(ShaderProgram* shdrPrg) : ObjectInstance(shdrPrg), initialized(false)
{
	//this->shaderProgram = shdrPrg;
	if (loadMultipleMeshes(RABBIT_MODEL, shdrPrg, geometries) != true) {
		std::cerr << "initializeModels(): Rabbit model loading failed." << std::endl;
	}
	else {
		//geometries[0]->texture= pgr::createTexture("\data\Spruce_obj");;
		initialized = true;
	}

	glm::mat4 location = glm::scale(globalModelMatrix, glm::vec3(RABBIT_SCALE));
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
