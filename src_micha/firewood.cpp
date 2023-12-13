#include <iostream>
#include "Firewood.h"

//const char* MODEL_NAME = "data/low_poly_tree/tree2.obj";

void Firewood::update(float elapsedTime, const glm::mat4* parentModelMatrix) {
  ObjectInstance::update(elapsedTime, parentModelMatrix);
}

void Firewood::collect_wood(int index) {
	std::list<glm::mat4>::iterator it=locations.begin();
	std::advance(it, index);
	locations.erase(it);
}

void Firewood::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
	if (initialized && (shaderProgram != nullptr)) {
		glUseProgram(shaderProgram->program);
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		for (auto geometry : geometries) {
			int i = 20;
			for (auto location : locations) {
				glStencilFunc(GL_ALWAYS, i++, 0xFF);
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
		glDisable(GL_STENCIL_TEST);
		glUseProgram(0);
		CHECK_GL_ERROR();
		

		
	}
	else {
		//std::cout << "Can't draw Firewood: triangle not initialized properly!" << std::endl;
	}
}

Firewood::Firewood(const char* obj_name, float spacing, glm::vec3 rotation, float rotation_size, glm::vec3 translation, float scale, ShaderProgram* shdrPrg) : ObjectInstance(shdrPrg), initialized(false)
{
	//this->shaderProgram = shdrPrg;
	if (loadMultipleMeshes(obj_name, shdrPrg, geometries) != true) {
		std::cerr << "initializeModels(): Firewood model loading failed." << std::endl;
	}
	else {
		initialized = true;
	}
	for (float i = -SCENE_WIDTH; i < SCENE_WIDTH; i+= spacing) {
		for (float j = -SCENE_WIDTH; j < SCENE_WIDTH; j+= spacing) {
			
			float x = i + (rand()%80 +10.0f)* spacing /100;
			float z = j +(rand() %80 +10.0f)* spacing /100;

			float randScale = (rand() % 40 + 80) / 100.0; //80-120%
			//float randRotation = (rand() % 200) / 100.0
			glm::mat4 location = glm::scale(globalModelMatrix, glm::vec3(randScale*scale));
			location = glm::translate(location, glm::vec3(x/ (randScale*scale), 1, z/ (randScale*scale)));
			location = glm::translate(location, translation);
			location = glm::rotate(location, rotation_size, rotation);
			//location = glm::rotate(location, randRotation, glm::vec3(0, 1, 0));
			
			
			locations.emplace_back(location);
		}
	}
	CHECK_GL_ERROR();
	
}
Firewood::~Firewood() {
	for (auto geometry : geometries) {
		glDeleteVertexArrays(1, &(geometry->vertexArrayObject));
		glDeleteBuffers(1, &(geometry->elementBufferObject));
		glDeleteBuffers(1, &(geometry->vertexBufferObject));
		delete geometry;
	}

	initialized = false;
}
