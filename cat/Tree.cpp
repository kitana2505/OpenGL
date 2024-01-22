#include <iostream>
#include "Tree.h"

//const char* MODEL_NAME = "data/low_poly_tree/tree2.obj";

void Tree::update(float elapsedTime, const glm::mat4* parentModelMatrix) {
  ObjectInstance::update(elapsedTime, parentModelMatrix);
}

void Tree::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
	if (initialized && (shaderProgram != nullptr)) {
		glUseProgram(shaderProgram->program);
		for (auto geometry : geometries) {
			for (auto location : locations) {
				
				glBindVertexArray(geometry->vertexArrayObject);
				setTransformUniforms(*shaderProgram, location*localModelMatrix, viewMatrix, projectionMatrix);
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
		std::cout << "Can't draw Tree: triangle not initialized properly!" << std::endl;
	}
}

Tree::Tree(ShaderProgram* shdrPrg) : ObjectInstance(shdrPrg), initialized(false)
{
	//this->shaderProgram = shdrPrg;
	if (loadMultipleMeshes(TREE_MODEL, shdrPrg, geometries) != true) {
		std::cerr << "initializeModels(): Tree model loading failed." << std::endl;
	}
	else {
		initialized = true;
	}
	
	glm::mat4 location = glm::scale(globalModelMatrix, glm::vec3(TREE_SCALE));
	location = glm::translate(location,TREE_LOCATION);
	locations.emplace_back(location);
	CHECK_GL_ERROR();
	
}
Tree::~Tree() {
	for (auto geometry : geometries) {
		glDeleteVertexArrays(1, &(geometry->vertexArrayObject));
		glDeleteBuffers(1, &(geometry->elementBufferObject));
		glDeleteBuffers(1, &(geometry->vertexBufferObject));
		delete geometry;
	}

	initialized = false;
}
