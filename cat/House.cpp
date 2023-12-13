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
	double spacing = 15.0f;
	double scale = 1.5f;

	glm::mat4 location = glm::scale(globalModelMatrix, glm::vec3(scale));
	locations.emplace_back(location);

	/*for (float i = -SCENE_WIDTH; i < SCENE_WIDTH; i += spacing) {
		for (float j = -SCENE_WIDTH; j < SCENE_WIDTH; j += spacing) {

			float x = i + (rand() % 80 + 10.0f) * spacing / 100;
			float z = j + (rand() % 80 + 10.0f) * spacing / 100;

			float randScale = (rand() % 40 + 80) / 100.0; //80-120%
			//float randRotation = (rand() % 200) / 100.0
			glm::mat4 location = glm::scale(globalModelMatrix, glm::vec3(randScale * scale));
			location = glm::translate(location, glm::vec3(x / (randScale * scale), 1, z / (randScale * scale)));
			// location = glm::translate(location, translation);
			// location = glm::rotate(location, rotation_size, rotation);
			//location = glm::rotate(location, randRotation, glm::vec3(0, 1, 0));


			locations.emplace_back(location);
		}
	}*/
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
