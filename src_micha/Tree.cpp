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
		//std::cout << "Can't draw Vegetation: triangle not initialized properly!" << std::endl;
	}
}

Tree::Tree(ShaderProgram* shdrPrg) : ObjectInstance(shdrPrg), initialized(false)
{
	//this->shaderProgram = shdrPrg;
	if (loadMultipleMeshes(TREE_MODEL, shdrPrg, geometries) != true) {
		std::cerr << "initializeModels(): Vegetation model loading failed." << std::endl;
	}
	else {
		//geometries[0]->texture= pgr::createTexture("\data\Spruce_obj");;
		initialized = true;
	}
	for (float i = -SCENE_WIDTH; i < SCENE_WIDTH; i+= TREE_SPACING) {
		for (float j = -SCENE_WIDTH; j < SCENE_WIDTH; j+= TREE_SPACING) {
			
			float x = i + (rand()%80 +10.0f)* TREE_SPACING /100;
			float z = j +(rand() %80 +10.0f)* TREE_SPACING /100;

			float randScale = (rand() % 40 + 80) / 100.0; //80-120%
			float randRotation = (rand() % 200) / 100.0;
			
			glm::mat4 location = glm::scale(globalModelMatrix, glm::vec3(randScale + TREE_SCALE));
			//location = glm::rotate(location, randRotation, glm::vec3(0, 1, 0));
			location = glm::translate(location, glm::vec3(x /( randScale*TREE_SCALE), 1,z / (randScale * TREE_SCALE)));
			location = glm::rotate(location, -1.6f, glm::vec3(1, 0, 0));;
			
			locations.emplace_back(location);
		}
	}
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
