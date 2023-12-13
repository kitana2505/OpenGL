#include <iostream>
#include "Vegetation.h"

//const char* MODEL_NAME = "data/low_poly_tree/tree2.obj";

void Vegetation::update(float elapsedTime, const glm::mat4* parentModelMatrix) {

  // propagate the update to children
  ObjectInstance::update(elapsedTime, parentModelMatrix);
}

void Vegetation::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
	if (initialized && (shaderProgram != nullptr)) {
		glUseProgram(shaderProgram->program);
		glEnable(GL_DEPTH_TEST);
		for (auto geometry : geometries) {
			for (auto location : locations) {
				
				glBindVertexArray(geometry->vertexArrayObject);
				CHECK_GL_ERROR();
				setTransformUniforms(*shaderProgram, location*localModelMatrix, viewMatrix, projectionMatrix);
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
				glBindVertexArray(0);

				
			}
		}
		glUseProgram(0);
		CHECK_GL_ERROR();
		

		
	}
	else {
		//std::cout << "Can't draw Vegetation: triangle not initialized properly!" << std::endl;
	}
}

Vegetation::Vegetation(const char* obj_name, float spacing, glm::vec3 rotation, float rotation_size, glm::vec3 translation, float scale, ShaderProgram* shdrPrg) : ObjectInstance(shdrPrg), initialized(false)
{
	//this->shaderProgram = shdrPrg;
	if (loadMultipleMeshes(obj_name, shdrPrg, geometries) != true) {
		std::cerr << "initializeModels(): Vegetation model loading failed." << std::endl;
	}
	else {
		//geometries[0]->texture= pgr::createTexture("\data\Spruce_obj");;
		initialized = true;
	}
	for (float i = -SCENE_WIDTH; i < SCENE_WIDTH; i+= spacing) {
		for (float j = -SCENE_WIDTH; j < SCENE_WIDTH; j+= spacing) {
			
			float x = i + (rand()%80 +10.0f)* spacing /100;
			float z = j +(rand() %80 +10.0f)* spacing /100;

			float randScale = (rand() % 40 + 80) / 100.0; //80-120%
			//float randRotation = (rand() % 200) / 100.0;
			
			glm::mat4 location = glm::scale(globalModelMatrix, glm::vec3(randScale*scale));
			location = glm::translate(location, glm::vec3(x * randScale, 1, z * randScale));
			location = glm::translate(location, translation);
			location = glm::rotate(location, rotation_size, rotation);
			//location = glm::rotate(location, randRotation, glm::vec3(0, 1, 0));
			
			
			locations.emplace_back(location);
		}
	}
	CHECK_GL_ERROR();
	
}
Vegetation::~Vegetation() {
	for (auto geometry : geometries) {
		glDeleteVertexArrays(1, &(geometry->vertexArrayObject));
		glDeleteBuffers(1, &(geometry->elementBufferObject));
		glDeleteBuffers(1, &(geometry->vertexBufferObject));
		delete geometry;
	}

	initialized = false;
}
