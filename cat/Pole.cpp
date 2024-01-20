#include <iostream>
#include <cmath>
#include <math.h>
#include "Pole.h"

const int PoleTrianglesCount = 4;
// temp constants used for PoleVertices array contents definition
const float invSqrt2v = (float)(1.0 / sqrt(2.0));

// Pole is represented as a tetrahedron (polyhedron with 4 faces and 4 vertices)
const float PoleVertices[] = {
	// non-interleaved array
	// vertices of tetrahedron, each face is an equilateral triangle, edge length 2.0

	// vertices
	//  1.0f,  0.0f, -invSqrt2v,   -> vertex 0
	//  0.0f,  1.0f,  invSqrt2v,   -> vertex 1
	// -1.0f,  0.0f, -invSqrt2v,   -> vertex 2
	//  0.0f, -1.0f,  invSqrt2v    -> vertex 3

	// three vertices per each triangle
	 0.0f, -1.0f,  invSqrt2v, // 3
	 1.0f,  0.0f, -invSqrt2v, // 0
	 0.0f,  1.0f,  invSqrt2v, // 1

	-1.0f,  0.0f, -invSqrt2v, // 2
	 0.0f, -1.0f,  invSqrt2v, // 3
	 0.0f,  1.0f,  invSqrt2v, // 1

	 1.0f,  0.0f, -invSqrt2v, // 0
	-1.0f,  0.0f, -invSqrt2v, // 2
	 0.0f,  1.0f,  invSqrt2v, // 1

	-1.0f,  0.0f, -invSqrt2v, // 2
	 1.0f,  0.0f, -invSqrt2v, // 0
	 0.0f, -1.0f,  invSqrt2v, // 3

	 // colors for vertices
	 1.0f, 0.0f, 1.0f, // 3
	 1.0f, 0.0f, 1.0f, // 0
	 0.0f, 1.0f, 0.0f, // 1

	 0.0f, 0.0f, 1.0f, // 2
	 1.0f, 0.0f, 1.0f, // 3
	 0.0f, 1.0f, 0.0f, // 1

	 1.0f, 0.0f, 1.0f, // 0
	 0.0f, 0.0f, 1.0f, // 2
	 0.0f, 1.0f, 0.0f, // 1

	 0.0f, 0.0f, 1.0f, // 2
	 1.0f, 0.0f, 1.0f, // 0
	 1.0f, 0.0f, 1.0f, // 3

	 // normals
	  1.0f, 0.0f, invSqrt2v,
	  1.0f, 0.0f, invSqrt2v,
	  1.0f, 0.0f, invSqrt2v,

	 -1.0f, 0.0f, invSqrt2v,
	 -1.0f, 0.0f, invSqrt2v,
	 -1.0f, 0.0f, invSqrt2v,

	  0.0f, 1.0f, -invSqrt2v,
	  0.0f, 1.0f, -invSqrt2v,
	  0.0f, 1.0f, -invSqrt2v,

	  0.0f, -1.0f, -invSqrt2v,
	  0.0f, -1.0f, -invSqrt2v,
	  0.0f, -1.0f, -invSqrt2v,
};

bool isVectorNullv(const glm::vec3& vect) {

	return !vect.x && !vect.y && !vect.z;
}

glm::mat4 Pole::alignObject(const glm::vec3& position, const glm::vec3& front, const glm::vec3& up) {

	glm::vec3 z = -glm::normalize(front);

	if (isVectorNullv(z))
		z = glm::vec3(0.0, 0.0, 1.0);

	glm::vec3 x = glm::normalize(glm::cross(up, z));

	if (isVectorNullv(x))
		x = glm::vec3(1.0, 0.0, 0.0);

	glm::vec3 y = glm::cross(z, x);
	//mat4 matrix = mat4(1.0f);
	glm::mat4 matrix = glm::mat4(
		x.x, x.y, x.z, 0.0,
		y.x, y.y, y.z, 0.0,
		z.x, z.y, z.z, 0.0,
		position.x, position.y, position.z, 1.0
	);

	return matrix;
}


void Pole::update(float elapsedTime, const glm::mat4* parentModelMatrix) {
	float timeDelta = elapsedTime - currentTime;
	currentTime = elapsedTime;

	ObjectInstance::update(elapsedTime, parentModelMatrix);
}

void Pole::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
	glm::mat4 modelMatrix = alignObject(position, direction, glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(size));

	const float frequency = POLE_ROT_FREQ; // per second
	const float angle = 0.5f * M_PI * frequency * (currentTime - startTime); // angle in radians
	localModelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0.0f, 0.0f, 1.0f));

	if (initialized && (shaderProgram != nullptr)) {
		glUseProgram(shaderProgram->program);
		for (auto geometry : geometries) {
			//for (auto location : locations) {

			glBindVertexArray(geometry->vertexArrayObject);
			//setTransformUniforms(*shaderProgram, location * localModelMatrix, viewMatrix, projectionMatrix);
			setTransformUniforms(*shaderProgram, localModelMatrix, viewMatrix, projectionMatrix);
			setMaterialUniforms(
				*shaderProgram,
				geometry->ambient,
				geometry->diffuse,
				geometry->specular,
				geometry->shininess,
				geometry->texture
			);
			//glDrawElements(GL_TRIANGLES, geometry->numTriangles * 3, GL_UNSIGNED_INT, 0);
			glDrawArrays(GL_TRIANGLES, 0, geometry->numTriangles * 3);
			glBindVertexArray(0);


			//}
		}
		glUseProgram(0);

	}
}


Pole* Pole::createPole(ShaderProgram* shdrPrg,const glm::vec3& PolePosition, float elapsedTime)
{
	float currentTime = 0.001f * (float)glutGet(GLUT_ELAPSED_TIME); // milliseconds => seconds

	Pole* newPole = new Pole(shdrPrg);

	//overwite if needed
	newPole->destroyed = false;
	newPole->startTime = elapsedTime;
	newPole->currentTime = newPole->startTime;
	newPole->size = POLE_SIZE;
	newPole->position = PolePosition;
	newPole->direction = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));
	return newPole;
}

Pole::Pole(ShaderProgram* shdrPrg) : ObjectInstance(shdrPrg), initialized(false)
{
	ObjectGeometry *geometry = new ObjectGeometry;

	glGenVertexArrays(1, &((geometry)->vertexArrayObject));
	glBindVertexArray((geometry)->vertexArrayObject);

	glGenBuffers(1, &((geometry)->vertexBufferObject));
	glBindBuffer(GL_ARRAY_BUFFER, (geometry)->vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(PoleVertices), PoleVertices, GL_STATIC_DRAW);
	CHECK_GL_ERROR();

	glEnableVertexAttribArray(shdrPrg->locations.position);
	// vertices of triangles - start at the beginning of the array
	glVertexAttribPointer(shdrPrg->locations.position, 3, GL_FLOAT, GL_FALSE, 0, 0);

	if (useLighting == false) {
		glEnableVertexAttribArray(shdrPrg->locations.color);
		// colors of vertices start after the positions
		glVertexAttribPointer(shdrPrg->locations.color, 3, GL_FLOAT, GL_FALSE, 0, (void*)(PoleTrianglesCount * 3 * 3 * sizeof(float)));
	}
	else {
		glEnableVertexAttribArray(shdrPrg->locations.normal);
		// normals of vertices start after the colors
		glVertexAttribPointer(shdrPrg->locations.normal, 3, GL_FLOAT, GL_FALSE, 0, (void*)(2 * PoleTrianglesCount * 3 * 3 * sizeof(float)));
	}

	(geometry)->ambient = glm::vec3(0.0f, 1.0f, 1.0f);
	(geometry)->diffuse = glm::vec3(0.0f, 1.0f, 1.0f);
	(geometry)->specular = glm::vec3(0.0f, 1.0f, 1.0f);
	(geometry)->shininess = 10.0f;
	(geometry)->texture = 0;

	glBindVertexArray(0);

	(geometry)->numTriangles = PoleTrianglesCount;
	geometries.push_back(geometry);

	initialized = true;
}

Pole::~Pole() {
	for (auto geometry : geometries) {
		glDeleteVertexArrays(1, &(geometry->vertexArrayObject));
		glDeleteBuffers(1, &(geometry->elementBufferObject));
		glDeleteBuffers(1, &(geometry->vertexBufferObject));
		delete geometry;
	}

	initialized = false;
}
