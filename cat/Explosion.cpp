#include <iostream>
#include <cmath>
#include <math.h>
#include "Explosion.h"
#include "data.h"
const int ExplosionTrianglesCount = 4;
// temp constants used for ExplosionVertices array contents definition

struct ExplosionShaderProgram {
	// identifier for the shader program
	GLuint program;              // = 0;
	// vertex attributes locations
	GLint posLocation;           // = -1;
	GLint texCoordLocation;      // = -1;
	// uniforms locations
	GLint PVMmatrixLocation;     // = -1;
	GLint VmatrixLocation;       // = -1;
	GLint timeLocation;          // = -1;
	GLint texSamplerLocation;    // = -1;
	GLint frameDurationLocation; // = -1;

} explosionShaderProgram;

void Explosion::update(float elapsedTime, const glm::mat4* parentModelMatrix) {
	ObjectInstance::update(elapsedTime, parentModelMatrix);
}

void Explosion::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
	glUseProgram(explosionShaderProgram.program);

	  // just take 3x3 rotatiosn part of the view transform
	glm::mat4 billboardRotationMatrix = glm::mat4(
		viewMatrix[0],
		viewMatrix[1],
		viewMatrix[2],
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
	);
	// inverse view rotation
	billboardRotationMatrix = glm::transpose(billboardRotationMatrix);

	glm::mat4 matrix = glm::mat4(1.0f);
	matrix = glm::translate(matrix, position);
	matrix = glm::scale(matrix, glm::vec3(size));

	matrix = matrix * billboardRotationMatrix; // make billboard to face the camera
	glm::mat4 PVMmatrix = projectionMatrix * viewMatrix * matrix;

	glUniformMatrix4fv(explosionShaderProgram.VmatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));   // view
	glUniformMatrix4fv(explosionShaderProgram.PVMmatrixLocation, 1, GL_FALSE, glm::value_ptr(PVMmatrix));  // model-view-projection
	glUniform1f(explosionShaderProgram.timeLocation, currentTime - startTime);
	glUniform1i(explosionShaderProgram.texSamplerLocation, 0);
	glUniform1f(explosionShaderProgram.frameDurationLocation, frameDuration);

	if (initialized && (shaderProgram != nullptr)) {
		glUseProgram(shaderProgram->program);
		for (auto geometry : geometries) {
			glBindVertexArray(geometry->vertexArrayObject);
			glBindTexture(GL_TEXTURE_2D, geometry->texture);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, geometry->numTriangles);
			glBindVertexArray(0);

			//}
		}
		glUseProgram(0);
	}

}

Explosion::Explosion(ShaderProgram* shdrPrg) : ObjectInstance(shdrPrg), initialized(false)
{
	ObjectGeometry *geometry = new ObjectGeometry;
	(geometry)->texture = pgr::createTexture(EXPLOSION_TEXTURE_NAME);

	glGenVertexArrays(1, &((geometry)->vertexArrayObject));
	glBindVertexArray((geometry)->vertexArrayObject);

	glGenBuffers(1, &((geometry)->vertexBufferObject)); \
		glBindBuffer(GL_ARRAY_BUFFER, (geometry)->vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(explosionVertexData), explosionVertexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(explosionShaderProgram.posLocation);
	// vertices of triangles - start at the beginning of the array (interlaced array)
	glVertexAttribPointer(explosionShaderProgram.posLocation, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);

	glEnableVertexAttribArray(explosionShaderProgram.texCoordLocation);
	// texture coordinates are placed just after the position of each vertex (interlaced array)
	glVertexAttribPointer(explosionShaderProgram.texCoordLocation, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindVertexArray(0);

	(geometry)->numTriangles = explosionNumQuadVertices;

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
