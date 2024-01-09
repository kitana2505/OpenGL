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



void Missile::update(float elapsedTime, const glm::mat4* parentModelMatrix) {
	ObjectInstance::update(elapsedTime, parentModelMatrix);
}

void Missile::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
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



/*	// align missile coordinate system to match its position and direction - see alignObject() function
	glm::mat4 modelMatrix = alignObject(missile->position, missile->direction, glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(missile->size));

	// angular speed = 2*pi*frequency => path = angular speed * time
	const float frequency = 2.0f; // per second
	const float angle = 2.0f * M_PI * frequency * (missile->currentTime - missile->startTime); // angle in radians
	modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0.0f, 0.0f, 1.0f));

	// send matrices to the vertex & fragment shader
	*/


}

Missile* Missile::createMissile(ShaderProgram* shdrPrg, MissileShaderProgram* missileShader,const glm::vec3& missilePosition, const glm::vec3& missileDirection, float& missileLaunchTime, float elapsedTime)
{
	float currentTime = 0.001f * (float)glutGet(GLUT_ELAPSED_TIME); // milliseconds => seconds
	if (currentTime - missileLaunchTime < MISSILE_LAUNCH_TIME_DELAY)
		return NULL;

	missileLaunchTime = currentTime;

	Missile* newMissile = new Missile(shdrPrg, missileShader);

	//overwite if needed
	//newMissile->destroyed = false;
	newMissile->startTime = elapsedTime;
	newMissile->currentTime = newMissile->startTime;
	//newMissile->size = MISSILE_SIZE;
	//newMissile->speed = MISSILE_SPEED;
	newMissile->position = missilePosition;
	newMissile->direction = glm::normalize(missileDirection);

	//gameObjects.missiles.push_back(newMissile);
	//objects.push_back(newMissile);
	return newMissile;
}

Missile::Missile(ShaderProgram* shdrPrg, MissileShaderProgram* missileShaderProgram)
{
	ObjectGeometry *geometry = new ObjectGeometry;

	glGenVertexArrays(1, &((geometry)->vertexArrayObject));
	glBindVertexArray((geometry)->vertexArrayObject);

	glGenBuffers(1, &((geometry)->vertexBufferObject));
	glBindBuffer(GL_ARRAY_BUFFER, (geometry)->vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(missileVertices), missileVertices, GL_STATIC_DRAW);
	CHECK_GL_ERROR();

	glEnableVertexAttribArray(missileShaderProgram->posLocation);
	// vertices of triangles - start at the beginning of the array
	glVertexAttribPointer(missileShaderProgram->posLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

	if (useLighting == false) {
		glEnableVertexAttribArray(missileShaderProgram->colorLocation);
		// colors of vertices start after the positions
		glVertexAttribPointer(missileShaderProgram->colorLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)(missileTrianglesCount * 3 * 3 * sizeof(float)));
	}
	else {
		glEnableVertexAttribArray(missileShaderProgram->normalLocation);
		// normals of vertices start after the colors
		glVertexAttribPointer(missileShaderProgram->normalLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)(2 * missileTrianglesCount * 3 * 3 * sizeof(float)));
	}

	(geometry)->ambient = glm::vec3(0.0f, 1.0f, 1.0f);
	(geometry)->diffuse = glm::vec3(0.0f, 1.0f, 1.0f);
	(geometry)->specular = glm::vec3(0.0f, 1.0f, 1.0f);
	(geometry)->shininess = 10.0f;
	(geometry)->texture = 0;

	glBindVertexArray(0);

	(geometry)->numTriangles = missileTrianglesCount;

	destroyed = false;
	startTime = 0;//gameState.elapsedTime;
	currentTime = 0;//newMissile->startTime;
	size = MISSILE_SIZE;
	speed = MISSILE_SPEED;
	position = glm::vec3(0.0f, 0.0f, 0.0f);//missilePosition;
	direction = glm::vec3(1.0f, 0.0f, 0.0f);//glm::normalize(missileDirection);

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
