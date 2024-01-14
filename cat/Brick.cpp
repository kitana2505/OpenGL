#include <iostream>
#include "Brick.h"
BrickShaderProgram* brickShdr;

void Brick::update(float elapsedTime, const glm::mat4* parentModelMatrix) {
	ObjectInstance::update(elapsedTime, parentModelMatrix);
}

void Brick::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
	if (initialized && (shaderProgram != nullptr)) {
		glUseProgram(brickShdr->program);
		for (auto geometry : geometries) {
			//for (auto location : locations) {
			position = BRICK_INITIAL_POS;
			glm::mat4 location = glm::scale(globalModelMatrix, glm::vec3(BRICK_SCALE));
			location = glm::translate(location, BRICK_INITIAL_POS);
			location = glm::rotate(location, BRICK_ROTATION, glm::vec3(0, 1, 0));
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


			//}
		}
		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);

	}
	else {
		//std::cout << "Can't draw Cat: triangle not initialized properly!" << std::endl;
	}
}

Brick::Brick(ShaderProgram* shdrPrg, BrickShaderProgram* brickshdrPrg) : ObjectInstance(shdrPrg),  initialized(false)
{
	brickShdr = brickshdrPrg;
	//this->shaderProgram = shdrPrg;
	if (loadMultipleMeshes(BRICK_MODEL, shdrPrg, geometries) != true) {
		std::cerr << "initializeModels():brick model loading failed." << std::endl;
	}
	else {
		//geometries[0]->texture= pgr::createTexture("\data\Spruce_obj");;
		initialized = true;
	}
	
	//locations.emplace_back(location);


	GLuint brickTex = pgr::createTexture("data/brick/brick.jpg"); // load textures by PGR framework
	GLuint mossTex = pgr::createTexture("data/brick/moss.png");
	glActiveTexture(GL_TEXTURE0); // select texture unit 0
	glBindTexture(GL_TEXTURE_2D, brickTex);
	glActiveTexture(GL_TEXTURE1); // select texture unit 1
	glBindTexture(GL_TEXTURE_2D, mossTex);
	// get locations of the uniform fragment shader attributes
	//GLint mossTexLoc = glGetUniformLocation(brickshdrPrg, "mossTex");
	//GLint brickTexLoc = glGetUniformLocation(brickshdrPrg, "brickTex");
	glUseProgram(brickshdrPrg->program);
	glUniform1i(brickshdrPrg->brickTex, 0); // brick texture is bound to texture unit 0
	glUniform1i(brickshdrPrg->mossTex, 1); // moss texture is bound to texture unit 1

	//TODO
	//glDrawElement()...?

	CHECK_GL_ERROR();
	initialized = true;
}
Brick::~Brick() {
	for (auto geometry : geometries) {
		glDeleteVertexArrays(1, &(geometry->vertexArrayObject));
		glDeleteBuffers(1, &(geometry->elementBufferObject));
		glDeleteBuffers(1, &(geometry->vertexBufferObject));
		delete geometry;
	}

	initialized = false;
}