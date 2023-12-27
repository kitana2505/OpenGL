#include <iostream>
#include "House.h"

//const char* MODEL_NAME = "data/low_poly_House/House2.obj";

void House::update(float elapsedTime, const glm::mat4* parentModelMatrix) {
	ObjectInstance::update(elapsedTime, parentModelMatrix);
}

void House::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
	if (initialized && (shaderProgram != nullptr)) {
		glUseProgram(shaderProgram->program);
		for (auto geometry : geometries) {
			for (auto location : locations) {

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
				glBindVertexArray(0);


			}
		}
		glUseProgram(0);

	}
	else {
		//std::cout << "Can't draw Vegetation: triangle not initialized properly!" << std::endl;
	}
}

House::House(ShaderProgram* shdrPrg) : ObjectInstance(shdrPrg), initialized(false)
{
	//this->shaderProgram = shdrPrg;
	if (loadMultipleMeshes(HOUSE_MODEL, shdrPrg, geometries) != true) {
		std::cerr << "initializeModels(): Vegetation model loading failed." << std::endl;
	}
	else {
		//geometries[0]->texture= pgr::createTexture("\data\Spruce_obj");;
		initialized = true;
	}
	glm::mat4 location = glm::scale(globalModelMatrix, glm::vec3(HOUSE_SCALE));
	location = glm::translate(location, glm::vec3(0.0f, HOUSE_SCALE /2,0.0f));
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
