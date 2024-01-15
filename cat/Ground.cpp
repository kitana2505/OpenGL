#include <iostream>
#include "Ground.h"

std::vector<glm::mat4> locations;
//const char* MODEL_NAME = "data/low_poly_Ground/Ground2.obj";

void Ground::update(float elapsedTime, const glm::mat4* parentModelMatrix) {
  ObjectInstance::update(elapsedTime, parentModelMatrix);
}

void Ground::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
	CHECK_GL_ERROR();
	if (initialized && (shaderProgram != nullptr)) {
		glUseProgram(shaderProgram->program);
		CHECK_GL_ERROR();
		glEnable(GL_DEPTH_TEST);
		CHECK_GL_ERROR();
		for (auto geometry : geometries) {
				glBindVertexArray(geometry->vertexArrayObject);
				for (auto location : locations) {

					glBindVertexArray(geometry->vertexArrayObject);
					CHECK_GL_ERROR();
					setTransformUniforms(*shaderProgram, location * localModelMatrix, viewMatrix, projectionMatrix);
					setMaterialUniforms(
						*shaderProgram,
						geometry->ambient,
						geometry->diffuse,
						geometry->specular,
						geometry->shininess,
						geometry->texture
					);
					CHECK_GL_ERROR();
					glDrawElements(GL_TRIANGLES, geometry->numTriangles * 3, GL_UNSIGNED_INT, 0);
				}
			glBindVertexArray(0);
		}
		glUseProgram(0);
		CHECK_GL_ERROR();
	}
	else {
		std::cout << "Can't draw Ground: Ground not initialized properly!" << std::endl;
	}
}

Ground::Ground(ShaderProgram* shdrPrg) : ObjectInstance(shdrPrg), initialized(false)
{
	//this->shaderProgram = shdrPrg;
	if (loadMultipleMeshes(GROUND_MODEL, shdrPrg, geometries) != true) {
		std::cerr << "initializeModels(): Vegetation model loading failed." << std::endl;
	}
	else {
		initialized = true;
		//this->localModelMatrix = glm::translate(localModelMatrix, GROUND_TRANSLATION);
		for (float i = -1.5*SCENE_WIDTH; i < 1.5 * SCENE_WIDTH; i += GROUND_SCALE.x) {
			for (float j = -1.5 * SCENE_WIDTH; j < 1.5 * SCENE_WIDTH; j += GROUND_SCALE.z) {
				glm::mat4 location = glm::scale(localModelMatrix, GROUND_SCALE);
				location = glm::translate(location, glm::vec3(i/ GROUND_SCALE.x, 0, j/ GROUND_SCALE.z));
				
				//location = glm::rotate(location, randRotation, glm::vec3(0, 1, 0));
				location = glm::rotate(location, GROUND_ROTATION, glm::vec3(1, 0, 0));

				locations.emplace_back(location);
			}
		}
	}
	CHECK_GL_ERROR();
	
}
Ground::~Ground() {
	for (auto geometry : geometries) {
		glDeleteVertexArrays(1, &(geometry->vertexArrayObject));
		glDeleteBuffers(1, &(geometry->elementBufferObject));
		glDeleteBuffers(1, &(geometry->vertexBufferObject));
		delete geometry;
	}

	initialized = false;
}
