#include <iostream>
#include <stdlib.h> 
#include <queue>
#include "Smoke.h"

float nextSmokeTimer = NEXT_SMOKE_TIME;
float deleteSmokeTimer = DELETE_SMOKE_TIME;
std::queue<float> smokeList;
int* current_fire_size;


void Smoke::update(float elapsedTime, const glm::mat4* parentModelMatrix) {

	float timeDelta = elapsedTime;
	nextSmokeTimer-= timeDelta;
	deleteSmokeTimer -= timeDelta;
	
	if (nextSmokeTimer < 0) {
		nextSmokeTimer = NEXT_SMOKE_TIME;
		smokeList.push(0.0f);
	}

	if (deleteSmokeTimer < 0) {
		deleteSmokeTimer = DELETE_SMOKE_TIME;
		if (smokeList.size() > SMOKE_COUNT) {
			smokeList.pop();
		}
	}

	for (int i = 0; i < smokeList.size(); i++) {
		float smokeTime = smokeList.front();
		smokeTime += timeDelta;
		smokeList.pop();
		smokeList.push(smokeTime);
	}
  // propagate the update to children
  ObjectInstance::update(elapsedTime, parentModelMatrix);
  ObjectInstance::update(elapsedTime, parentModelMatrix);
}
void Smoke::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
	if (initialized) {
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		for (auto geometry : geometries) {

			for (int i = 0; i < smokeList.size(); i++) {
				glStencilFunc(GL_ALWAYS, i+2, 0xFF);
				float smokeTime = smokeList.front();
				float smokeHeight = smokeTime* SMOKE_SPEED;
				glm::vec3 smoke_position;
				smoke_position.x = sin(smokeTime* SMOKE_ROTATION_SPEED)* SMOKE_ROTATION;
				smoke_position.y = smokeTime * SMOKE_SPEED;
				smoke_position.z = cos(smokeTime* SMOKE_ROTATION_SPEED)*SMOKE_ROTATION;
				float smokeScale = smokeTime * SMOKE_SCALE*flame_sizes[*current_fire_size];
				glUseProgram(shdr->program);
				glUniform1f(shdr->scale, smokeScale);
				glm::mat4 smokeMatrix = glm::translate(this->globalModelMatrix, smoke_position);
				glm::mat4 location = smokeMatrix;
				glm::mat4 PVM = projectionMatrix * viewMatrix * smokeMatrix;
				draw_smoke(viewMatrix, projectionMatrix, *geometry, location);

				smokeList.pop();
				smokeList.push(smokeTime);
			}
		}
		glDisable(GL_STENCIL_TEST);
		glBindVertexArray(0);

		glUseProgram(0);
	}
}
void Smoke::pop_smoke(int index)
{
	for (int i = 0; i < smokeList.size(); i++) {
		float smokeTime = smokeList.front();
		smokeList.pop();
		if (i != index) {
			smokeList.push(smokeTime);
		}
	}
}
void Smoke::draw_smoke(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, ObjectGeometry geometry, glm::mat4 location)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	glUseProgram(shdr->program);

	glm::mat4 billboardRotationMatrix = glm::mat4(
		viewMatrix[0],
		viewMatrix[1],
		viewMatrix[2],
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
	);
	// inverse view rotation
	billboardRotationMatrix = glm::transpose(billboardRotationMatrix);

	glm::mat4 matrix = location;
	matrix = glm::scale(matrix, glm::vec3(0.7f));
	matrix = glm::translate(matrix, glm::vec3(0, 2, 0));

	//glm::mat4 matrix = globalModelMatrix;
	matrix = matrix * billboardRotationMatrix; // make billboard to face the camera
	glm::mat4 PVMmatrix = projectionMatrix * viewMatrix * matrix;

	glUniformMatrix4fv(shdr->VmatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));   // view

	glUniformMatrix4fv(shdr->PVMmatrixLocation, 1, GL_FALSE, glm::value_ptr(PVMmatrix));  // model-view-projection
	glUniform2i(shdr->frames, 1, 1);
	glUniform1f(shdr->timeLocation, 0);

	glBindVertexArray(geometry.vertexArrayObject);
	glBindTexture(GL_TEXTURE_2D, geometry.texture);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, geometry.numTriangles);

	glBindVertexArray(0);
	glUseProgram(0);

	glDisable(GL_BLEND);

	return;
}

Smoke::Smoke(FireShaderProgram* shdrPrg, int*currentfiresize)
{
	current_fire_size = currentfiresize;
	initialized = true;
	this->shdr = shdrPrg;
	ObjectGeometry* geometry = new ObjectGeometry;
	geometry->texture = pgr::createTexture(SMOKE_TEXTURE_NAME);
	CHECK_GL_ERROR();
	glGenVertexArrays(1, &(geometry->vertexArrayObject));
	glBindVertexArray(geometry->vertexArrayObject);

	glGenBuffers(1, &(geometry->vertexBufferObject)); \
		glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(flameVertexData), flameVertexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(shdrPrg->posLocation);
	// vertices of triangles - start at the beginning of the array (interlaced array)
	glVertexAttribPointer(shdrPrg->posLocation, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);

	glEnableVertexAttribArray(shdrPrg->texCoordLocation);
	// texture coordinates are placed just after the position of each vertex (interlaced array)
	CHECK_GL_ERROR();
	glVertexAttribPointer(shdrPrg->texCoordLocation, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	CHECK_GL_ERROR();
	glBindVertexArray(0);

	geometry->numTriangles = flameNumQuadVertices;
	geometries.push_back(geometry);
	CHECK_GL_ERROR();
	//if (loadMultipleMeshes(SMOKE_MODEL_NAME, shdrPrg, geometries) != true) {
	//	std::cerr << "initializeModels(): Smoke model loading failed." << std::endl;
	//	
	//}
	//else {
	//	/*
	//	this->globalModelMatrix = glm::scale(this->localModelMatrix, glm::vec3(0.1f));
	//	this->globalModelMatrix = glm::translate(this->globalModelMatrix, glm::vec3(0, 5.0f, 0));*/
	//	//this->localModelMatrix = glm::translate(this->localModelMatrix, glm::vec3(0,0.05f,0));
	//}
	CHECK_GL_ERROR();
}

Smoke::~Smoke() {
	for (auto geometry : geometries) {
		glDeleteVertexArrays(1, &(geometry->vertexArrayObject));
		glDeleteBuffers(1, &(geometry->elementBufferObject));
		glDeleteBuffers(1, &(geometry->vertexBufferObject));
		delete geometry;
	}

	initialized = false;
}
