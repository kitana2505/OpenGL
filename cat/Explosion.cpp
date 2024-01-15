#include <iostream>
#include <cmath>
#include <math.h>
#include "Explosion.h"
#include "data.h"
const int ExplosionTrianglesCount = 4;
// temp constants used for ExplosionVertices array contents definition
//float elapsed_time3 = 0.0f;
ExplosionShaderProgram* explosionShader;


void Explosion::update(float elapsedTime, const glm::mat4* parentModelMatrix) {
	//elapsed_time3 += currentTime;
	currentTime = elapsedTime;
	ObjectInstance::update(elapsedTime, parentModelMatrix);
}

void Explosion::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glUseProgram(explosionShader->program);
	
	  // just take 3x3 rotatiosn part of the view transform
	glm::mat4 billboardRotationMatrix = glm::mat4(
		viewMatrix[0],
		viewMatrix[1],
		viewMatrix[2],
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
	);
	// inverse view rotation
	billboardRotationMatrix = glm::transpose(billboardRotationMatrix);

	//position = glm::vec3(0.0f, 1.0f, 0.0f); // position of fire
	//size = 1.0f;
	glm::mat4 matrix = this->localModelMatrix;
	matrix = glm::translate(matrix, position);
	matrix = glm::scale(matrix, glm::vec3(size));

	matrix = matrix * billboardRotationMatrix; // make billboard to face the camera
	glm::mat4 PVMmatrix = projectionMatrix * viewMatrix * matrix;

	glUniformMatrix4fv(explosionShader->VmatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));   // view
	glUniformMatrix4fv(explosionShader->PVMmatrixLocation, 1, GL_FALSE, glm::value_ptr(PVMmatrix));  // model-view-projection
	glUniform1f(explosionShader->timeLocation, currentTime-startTime);
	glUniform1i(explosionShader->texSamplerLocation, 0);
	glUniform1f(explosionShader->frameDurationLocation, frameDuration);
	glUniform2i(explosionShader->frames, 8, 2);
	glUniform1f(explosionShader->scale, 1.0f);

	if (initialized && (explosionShader != nullptr)) {
		glUseProgram(explosionShader->program);
		for (auto geometry : geometries) {
			glBindVertexArray(geometry->vertexArrayObject);
			glBindTexture(GL_TEXTURE_2D, geometry->texture);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, geometry->numTriangles);
			glBindVertexArray(0);

			//}
		}
		glUseProgram(0);
	}
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

Explosion::Explosion(ShaderProgram* shdrPrg, ExplosionShaderProgram* explosionShaderProgram) : ObjectInstance(shdrPrg), initialized(false)
{	
	ObjectGeometry* geometry = new ObjectGeometry;
	(geometry)->texture = pgr::createTexture(EXPLOSION_TEXTURE_NAME);
	explosionShader = explosionShaderProgram;
	glGenVertexArrays(1, &((geometry)->vertexArrayObject));
	glBindVertexArray((geometry)->vertexArrayObject);

	glGenBuffers(1, &((geometry)->vertexBufferObject)); \
		glBindBuffer(GL_ARRAY_BUFFER, (geometry)->vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(explosionVertexData), explosionVertexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(explosionShaderProgram->posLocation);
	// vertices of triangles - start at the beginning of the array (interlaced array)
	glVertexAttribPointer(explosionShaderProgram->posLocation, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);

	glEnableVertexAttribArray(explosionShaderProgram->texCoordLocation);
	// texture coordinates are placed just after the position of each vertex (interlaced array)
	glVertexAttribPointer(explosionShaderProgram->texCoordLocation, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindVertexArray(0);

	(geometry)->numTriangles = explosionNumQuadVertices;
	geometries.push_back(geometry);
	initialized = true;
}

Explosion::~Explosion() {
	for (auto geometry : geometries) {
		glDeleteVertexArrays(1, &(geometry->vertexArrayObject));
		glDeleteBuffers(1, &(geometry->elementBufferObject));
		glDeleteBuffers(1, &(geometry->vertexBufferObject));
		delete geometry;
	}

	initialized = false;
}
