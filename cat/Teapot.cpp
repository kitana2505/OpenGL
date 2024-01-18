#include <iostream>
#include "Teapot.h"


void Teapot::update(float elapsedTime, const glm::mat4* parentModelMatrix) {
	ObjectInstance::update(elapsedTime, parentModelMatrix);
}

void Teapot::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
	if (initialized && (shaderProgram != nullptr)) {
		glUseProgram(shaderProgram->program);
		for (auto geometry : geometries) {
			glBindVertexArray(geometry->vertexArrayObject);

			//glm::mat4 projection = glm::rotate(glm::mat4(1.0f), glm::radians(currentTime * 30.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // the animation
			//glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(0.6f));
			//glUniformMatrix4fv(shaderProgram->locations.PVMmatrix, 1, GL_FALSE, glm::value_ptr(projection * model));


			//glm::mat4 modelMatrix = alignObject(position, direction, glm::vec3(0.0f, 0.0f, 1.0f));
			size = 0.5f;
			glm::mat4 modelMatrix = glm::scale(localModelMatrix, glm::vec3(size));

			// angular speed = 2*pi*frequency => path = angular speed * time
			const float frequency = 0.25f; // per second
			const float angle = 5.0f * M_PI * frequency * (currentTime - startTime); // angle in radians
			localModelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0.0f, 1.0f, 0.0f));
			localModelMatrix = glm::translate(localModelMatrix, MUSHROOM_INITIAL_POS);
			setTransformUniforms(*shaderProgram, localModelMatrix, viewMatrix, projectionMatrix);
			setMaterialUniforms(
				*shaderProgram,
				geometry->ambient,
				geometry->diffuse,
				geometry->specular,
				geometry->shininess,
				geometry->texture
			);
			//glDrawArrays(GL_TRIANGLES, 0, (geometry)->numTriangles);
			glDrawElements(GL_TRIANGLES, (geometry)->numTriangles * 3, GL_UNSIGNED_INT, (void*)0);
			glBindVertexArray(0);
			glUseProgram(0);
		}
	}
	else {
		//std::cout << "Can't draw Cat: triangle not initialized properly!" << std::endl;
	}
}

Teapot::Teapot(ShaderProgram* shdrPrg) : ObjectInstance(shdrPrg), initialized(false)
{
	ObjectGeometry* geometry = new ObjectGeometry;
	glGenVertexArrays(1, &((geometry)->vertexArrayObject));
	glBindVertexArray((geometry)->vertexArrayObject);
	// vertex positions
	glGenBuffers(1, &((geometry)->vertexBufferObject));
	glBindBuffer(GL_ARRAY_BUFFER, (geometry)->vertexBufferObject);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh.nVertices * mesh.nAttribsPerVertex, mesh.verticesInterleaved, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	
	glEnableVertexAttribArray(shdrPrg->locations.position);
	glVertexAttribPointer(shdrPrg->locations.position, 3, GL_FLOAT, GL_FALSE, mesh.nAttribsPerVertex * sizeof(float), (void*)(0));  // [xyz][nx,ny,nz][s,t]
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (geometry)->vertexBufferObject);
	// buffer for triangle indices - ELEMENT_ARRAY
	glGenBuffers(1, &((geometry)->vertexBufferObject));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (geometry)->vertexBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * mesh.nTriangles * 3, mesh.triangles, GL_STATIC_DRAW);  // 3-indices per triangle
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	(geometry)->numTriangles = mesh.nTriangles;
	
	glBindVertexArray(0);
	geometries.push_back(geometry);
	CHECK_GL_ERROR();
	initialized = true;
}
Teapot::~Teapot() {
	for (auto geometry : geometries) {
		glDeleteVertexArrays(1, &(geometry->vertexArrayObject));
		glDeleteBuffers(1, &(geometry->elementBufferObject));
		glDeleteBuffers(1, &(geometry->vertexBufferObject));
		delete geometry;
	}

	initialized = false;
}
