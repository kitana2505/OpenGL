#include <iostream>
#include <cmath>
#include <math.h>
#include "Missile.h"
const int missileTrianglesCount = 4;
// temp constants used for missileVertices array contents definition
const float invSqrt2 = (float)(1.0 / sqrt(2.0));

// missile is represented as a tetrahedron (polyhedron with 4 faces and 4 vertices)
const float missileVertices[] = {
	// non-interleaved array
	// vertices of tetrahedron, each face is an equilateral triangle, edge length 2.0

	// vertices
	//  1.0f,  0.0f, -invSqrt2,   -> vertex 0
	//  0.0f,  1.0f,  invSqrt2,   -> vertex 1
	// -1.0f,  0.0f, -invSqrt2,   -> vertex 2
	//  0.0f, -1.0f,  invSqrt2    -> vertex 3

	// three vertices per each triangle
	 0.0f, -1.0f,  invSqrt2, // 3
	 1.0f,  0.0f, -invSqrt2, // 0
	 0.0f,  1.0f,  invSqrt2, // 1

	-1.0f,  0.0f, -invSqrt2, // 2
	 0.0f, -1.0f,  invSqrt2, // 3
	 0.0f,  1.0f,  invSqrt2, // 1

	 1.0f,  0.0f, -invSqrt2, // 0
	-1.0f,  0.0f, -invSqrt2, // 2
	 0.0f,  1.0f,  invSqrt2, // 1

	-1.0f,  0.0f, -invSqrt2, // 2
	 1.0f,  0.0f, -invSqrt2, // 0
	 0.0f, -1.0f,  invSqrt2, // 3

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
	  1.0f, 0.0f, invSqrt2,
	  1.0f, 0.0f, invSqrt2,
	  1.0f, 0.0f, invSqrt2,

	 -1.0f, 0.0f, invSqrt2,
	 -1.0f, 0.0f, invSqrt2,
	 -1.0f, 0.0f, invSqrt2,

	  0.0f, 1.0f, -invSqrt2,
	  0.0f, 1.0f, -invSqrt2,
	  0.0f, 1.0f, -invSqrt2,

	  0.0f, -1.0f, -invSqrt2,
	  0.0f, -1.0f, -invSqrt2,
	  0.0f, -1.0f, -invSqrt2,
};

bool isVectorNull(const glm::vec3& vect) {

	return !vect.x && !vect.y && !vect.z;
}

glm::mat4 Missile::alignObject(const glm::vec3& position, const glm::vec3& front, const glm::vec3& up) {

	glm::vec3 z = -glm::normalize(front);

	if (isVectorNull(z))
		z = glm::vec3(0.0, 0.0, 1.0);

	glm::vec3 x = glm::normalize(glm::cross(up, z));

	if (isVectorNull(x))
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


void Missile::update(float elapsedTime, const glm::mat4* parentModelMatrix) {
	float timeDelta = elapsedTime - currentTime;

	currentTime = elapsedTime;
	position += timeDelta * speed * direction;
	
	if ((currentTime - startTime) * speed > MISSILE_MAX_DISTANCE)
		destroyed = true;

	ObjectInstance::update(elapsedTime, parentModelMatrix);
}

void Missile::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
	glm::mat4 modelMatrix = alignObject(position, direction, glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(size));

	// angular speed = 2*pi*frequency => path = angular speed * time
	const float frequency = 1.0f; // per second
	const float angle = 5.0f * M_PI * frequency * (currentTime - startTime); // angle in radians
	localModelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0.0f, 0.0f, 1.0f));

	if (initialized && (shaderProgram != nullptr)) {
		glUseProgram(shaderProgram->program);
		for (auto geometry : geometries) {

			glBindVertexArray(geometry->vertexArrayObject);
			
			setTransformUniforms(*shaderProgram, localModelMatrix, viewMatrix, projectionMatrix);
			setMaterialUniforms(
				*shaderProgram,
				geometry->ambient,
				geometry->diffuse,
				geometry->specular,
				geometry->shininess,
				geometry->texture
			);
			
			glDrawArrays(GL_TRIANGLES, 0, geometry->numTriangles * 3);
			glBindVertexArray(0);


		}
		glUseProgram(0);

	}
}


Missile* Missile::createMissile(ShaderProgram* shdrPrg,const glm::vec3& missilePosition, const glm::vec3& missileDirection, float elapsedTime)
{
	float currentTime = 0.001f * (float)glutGet(GLUT_ELAPSED_TIME); // milliseconds => seconds

	Missile* newMissile = new Missile(shdrPrg);

	//overwite if needed
	newMissile->destroyed = false;
	newMissile->startTime = elapsedTime;
	newMissile->currentTime = newMissile->startTime;
	newMissile->size = MISSILE_SIZE;
	newMissile->speed = MISSILE_SPEED;
	newMissile->position = missilePosition;
	newMissile->direction = glm::normalize(missileDirection);
	
	return newMissile;
}

Missile::Missile(ShaderProgram* shdrPrg) : ObjectInstance(shdrPrg), initialized(false)
{
	ObjectGeometry *geometry = new ObjectGeometry;

	glGenVertexArrays(1, &((geometry)->vertexArrayObject));
	glBindVertexArray((geometry)->vertexArrayObject);

	glGenBuffers(1, &((geometry)->vertexBufferObject));
	glBindBuffer(GL_ARRAY_BUFFER, (geometry)->vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(missileVertices), missileVertices, GL_STATIC_DRAW);
	CHECK_GL_ERROR();

	glEnableVertexAttribArray(shdrPrg->locations.position);
	// vertices of triangles - start at the beginning of the array
	glVertexAttribPointer(shdrPrg->locations.position, 3, GL_FLOAT, GL_FALSE, 0, 0);

	if (useLighting == false) {
		glEnableVertexAttribArray(shdrPrg->locations.color);
		// colors of vertices start after the positions
		glVertexAttribPointer(shdrPrg->locations.color, 3, GL_FLOAT, GL_FALSE, 0, (void*)(missileTrianglesCount * 3 * 3 * sizeof(float)));
	}
	else {
		glEnableVertexAttribArray(shdrPrg->locations.normal);
		// normals of vertices start after the colors
		glVertexAttribPointer(shdrPrg->locations.normal, 3, GL_FLOAT, GL_FALSE, 0, (void*)(2 * missileTrianglesCount * 3 * 3 * sizeof(float)));
	}

	(geometry)->ambient = glm::vec3(0.0f, 1.0f, 1.0f);
	(geometry)->diffuse = glm::vec3(0.0f, 1.0f, 1.0f);
	(geometry)->specular = glm::vec3(0.0f, 1.0f, 1.0f);
	(geometry)->shininess = 10.0f;
	(geometry)->texture = 0;

	glBindVertexArray(0);

	(geometry)->numTriangles = missileTrianglesCount;
	geometries.push_back(geometry);

	initialized = true;
}

Missile::~Missile() {
	for (auto geometry : geometries) {
		glDeleteVertexArrays(1, &(geometry->vertexArrayObject));
		glDeleteBuffers(1, &(geometry->elementBufferObject));
		glDeleteBuffers(1, &(geometry->vertexBufferObject));
		delete geometry;
	}

	initialized = false;
}
