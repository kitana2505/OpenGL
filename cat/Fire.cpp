#include <iostream>
#include "Fire.h"
#include "data/fire_wood/fireplace.h"
const char* FIRE_MODEL_NAME = "data/low_poly_fireplace/fireplace.obj";

ObjectGeometry* flames= new ObjectGeometry;
ObjectGeometry* fireWood = new ObjectGeometry;
FireShaderProgram* fireShader;
float elapsed_time=0.0f;
glm::mat4 fireWoodLocation;
float firesize = 10;
int current_fire_size = 4;

void Fire::update(float eTime, const glm::mat4* parentModelMatrix) {
	firesize -= eTime*FIRE_BURN_MULT;
	if(firesize < 0) {
		firesize = 0;
	}
	elapsed_time += eTime;
	update_fire_size();
	ObjectInstance::update(eTime, parentModelMatrix);
}

void Fire::update_fire_size()
{
	if (firesize < MEDIUM_FIRE_THRESHOLD && current_fire_size != 0) {
		
		current_fire_size = 0;
		fireWoodLocation = localModelMatrix;
		fireWoodLocation = glm::scale(fireWoodLocation, FIRE_SMALL_SCALE);
		fireWoodLocation = glm::translate(fireWoodLocation, FIRE_SMALL_TRANSLATE);
		fireWoodLocation = glm::rotate(fireWoodLocation, -1.6f, FIRE_ROTATE);
		glUseProgram(shaderProgram->program);
		glUniform1f(shaderProgram->locations.fireStrength, SMALL_FIRE_STRENGTH);
		glUniform1f(shaderProgram->locations.fireFallof, 2.0f);
		glUseProgram(0);
	}
	else if (firesize < HIGH_FIRE_THRESHOLD && firesize >= MEDIUM_FIRE_THRESHOLD && current_fire_size != 1) {
		current_fire_size = 1;
		fireWoodLocation = localModelMatrix;
		fireWoodLocation = glm::scale(fireWoodLocation, FIRE_MEDIUM_SCALE);
		fireWoodLocation = glm::translate(fireWoodLocation, FIRE_MEDIUM_TRANSLATE);
		fireWoodLocation = glm::rotate(fireWoodLocation, -1.6f, FIRE_ROTATE);
		glUseProgram(shaderProgram->program);
		glUniform1f(shaderProgram->locations.fireStrength, MEDIUM_FIRE_STRENGTH);
		glUniform1f(shaderProgram->locations.fireFallof, 1.4f);
		glUseProgram(0);
	}
	else if (firesize >= HIGH_FIRE_THRESHOLD && current_fire_size != 2) {
		current_fire_size = 2;
		fireWoodLocation = localModelMatrix;
		fireWoodLocation = glm::scale(fireWoodLocation, FIRE_BIG_SCALE);
		fireWoodLocation = glm::translate(fireWoodLocation, FIRE_BIG_TRANSLATE);
		fireWoodLocation = glm::rotate(fireWoodLocation, -1.6f, FIRE_ROTATE);
		glUseProgram(shaderProgram->program);
		glUniform1f(shaderProgram->locations.fireStrength, HIGH_FIRE_STRENGTH);
		glUniform1f(shaderProgram->locations.fireFallof, 0.8f);
		glUseProgram(0);
	}
}

void Fire::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	
	if (initialized && (shaderProgram != nullptr)) {
		glUseProgram(shaderProgram->program);
		glm::vec3 firelocation = glm::vec3((viewMatrix * glm::vec4(glm::vec3(0.0f, 1.0f, 0.0f), 1.0f)));
		glUniform3f(shaderProgram->locations.firePosition, firelocation.x, firelocation.y, firelocation.z);
		for (auto geometry : geometries) {
			glBindVertexArray(geometry->vertexArrayObject);
			setTransformUniforms(*shaderProgram, globalModelMatrix, viewMatrix, projectionMatrix);
			setMaterialUniforms(
				*shaderProgram,
				geometry->ambient,
				geometry->diffuse,
				geometry->specular,
				geometry->shininess,
				geometry->texture
			);
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glDrawElements(GL_TRIANGLES, geometry->numTriangles * 3, GL_UNSIGNED_INT, 0);
		}
		
		glBindVertexArray(0);

		glUseProgram(0);
		draw_fireWood(viewMatrix, projectionMatrix);
		draw_flames(viewMatrix,projectionMatrix);
		glDisable(GL_STENCIL_TEST);
		ObjectInstance::draw(viewMatrix, projectionMatrix);
	}
	else {
		std::cout << "Can't draw Fire: Fire not initialized properly!" << std::endl;
	}
}
void Fire::draw_flames(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	// ========  END OF SOLUTION - TASK 4_2-2  ======== //

	glUseProgram(fireShader->program);

	// version 1: inversion of the rotation part of the view matrix

	  // just take 3x3 rotation part of the view transform
	glm::mat4 billboardRotationMatrix = glm::mat4(
		viewMatrix[0],
		viewMatrix[1],
		viewMatrix[2],
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
	);
	// inverse view rotation
	billboardRotationMatrix = glm::transpose(billboardRotationMatrix);

	glm::mat4 matrix = this->localModelMatrix;
	matrix = glm::scale(matrix, glm::vec3(0.8f));
	matrix = glm::translate(matrix, glm::vec3(0, flame_sizes[current_fire_size], 0));
	
	//glm::mat4 matrix = globalModelMatrix;
	matrix = matrix * billboardRotationMatrix; // make billboard to face the camera
	glm::mat4 PVMmatrix = projectionMatrix * viewMatrix * matrix;

	glUniformMatrix4fv(fireShader->VmatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));   // view

	glUniformMatrix4fv(fireShader->PVMmatrixLocation, 1, GL_FALSE, glm::value_ptr(PVMmatrix));  // model-view-projection
	glUniform1f(fireShader->timeLocation, elapsed_time);
	glUniform1i(fireShader->texSamplerLocation, 0);
	glUniform2i(fireShader->frames, 3,2);
	glUniform1f(fireShader->frameDurationLocation, 0.5f);
	glUniform1f(fireShader->scale, flame_sizes[current_fire_size]);

	glBindVertexArray(flames->vertexArrayObject);
	glBindTexture(GL_TEXTURE_2D, flames->texture);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, flames->numTriangles);

	glBindVertexArray(0);
	glUseProgram(0);

	glDisable(GL_BLEND);

	return;
}

void Fire::add_wood() {
	firesize += 1;
}

Fire::Fire(ShaderProgram* shdrPrg, FireShaderProgram* fireShaderProgram )
{
	this->shaderProgram = shdrPrg;
	fireShader = fireShaderProgram;
	if (loadMultipleMeshes(FIRE_MODEL_NAME, shdrPrg, geometries) != true) {
		std::cerr << "initializeModels(): Fire model loading failed." << std::endl;
	}
	else {
		initialized = true;
		this->localModelMatrix = glm::scale(localModelMatrix, FIRE_SCALE);
		this->localModelMatrix = glm::translate(localModelMatrix, glm::vec3(FIRE_TRANSLATE));
		glUseProgram(shaderProgram->program);
		glUniform3f(shaderProgram->locations.fireAmbient, 0.0f,0.0f,0.0f);
		glUniform3f(shaderProgram->locations.fireDiffuse, 1.0f, 0.4f, 0.0f);
		glUniform3f(shaderProgram->locations.fireSpecular, 1.5f, 0.5f, 0.0f);
		glUniform3f(shaderProgram->locations.firePosition, 0, 0, 0);
		glUniform1f(shaderProgram->locations.fireStrength, 5);
		glUniform1f(shaderProgram->locations.fireFallof, 1.5f);
		glUseProgram(0);
		
	}

	//initialize flames
	flames->texture = pgr::createTexture(EXPLOSION_TEXTURE_NAME);
	CHECK_GL_ERROR();
	glGenVertexArrays(1, &(flames->vertexArrayObject));
	glBindVertexArray(flames->vertexArrayObject);

	glGenBuffers(1, &(flames->vertexBufferObject)); \
		glBindBuffer(GL_ARRAY_BUFFER, flames->vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(flameVertexData), flameVertexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(fireShaderProgram->posLocation);
	// vertices of triangles - start at the beginning of the array (interlaced array)
	glVertexAttribPointer(fireShaderProgram->posLocation, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	
	glEnableVertexAttribArray(fireShaderProgram->texCoordLocation);
	// texture coordinates are placed just after the position of each vertex (interlaced array)
	CHECK_GL_ERROR();
	glVertexAttribPointer(fireShaderProgram->texCoordLocation, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	CHECK_GL_ERROR();
	glBindVertexArray(0);

	flames->numTriangles = flameNumQuadVertices;
	init_fireWood();
	
}

void Fire::draw_fireWood(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {
	glUseProgram(shaderProgram->program);
	setTransformUniforms(*shaderProgram, fireWoodLocation, viewMatrix, projectionMatrix);
	setMaterialUniforms(
		*shaderProgram,
		fireWood->ambient,
		fireWood->diffuse,
		fireWood->specular,
		fireWood->shininess,
		0
	);
	CHECK_GL_ERROR();
	glBindVertexArray(fireWood->vertexArrayObject);
	if (firesize < MEDIUM_FIRE_THRESHOLD) {
		glDrawElements(GL_TRIANGLES, fire_smallNTriangles * 3, GL_UNSIGNED_INT, 0);
	}
	else if (firesize < HIGH_FIRE_THRESHOLD) {
		glDrawElements(GL_TRIANGLES, fire_mediumNTriangles * 3, GL_UNSIGNED_INT, (void*)((fire_smallNTriangles * 3) * sizeof(unsigned int)));
	}
	else {
		glDrawElements(GL_TRIANGLES, fire_bigNTriangles * 3, GL_UNSIGNED_INT, (void*)((fire_smallNTriangles * 3 + fire_mediumNTriangles * 3) * sizeof(unsigned int)));
	}
	
	CHECK_GL_ERROR();

	glBindVertexArray(0);
	glUseProgram(0);
}

void Fire::init_fireWood() {
	for (int i = 0; i < fire_smallNTriangles*3; i++) {
		fire_Triangles[i] = fire_smallTriangles[i];
	}
	for (int i = 0; i < fire_mediumNTriangles*3; i++) {
		fire_Triangles[i + fire_smallNTriangles*3] = fire_mediumTriangles[i] + fire_smallNVertices;
	}
	for (int i = 0; i < fire_bigNTriangles*3; i++) {
		fire_Triangles[i + fire_smallNTriangles*3+ fire_mediumNTriangles*3] = fire_bigTriangles[i] + fire_smallNVertices+ fire_mediumNVertices;
	}
	for (int i = 0; i < fire_smallNVertices*8; i++) {
		fire_Vertices[i] = fire_smallVertices[i];
	}for (int i = 0; i < fire_mediumNVertices*8; i++) {
		fire_Vertices[i + fire_smallNVertices*8] = fire_mediumVertices[i];
	}for (int i = 0; i < fire_bigNVertices*8; i++) {
		fire_Vertices[i + fire_smallNVertices*8 + fire_mediumNVertices*8] = fire_bigVertices[i];
	}
	glGenVertexArrays(1, &((fireWood)->vertexArrayObject));
	glBindVertexArray((fireWood)->vertexArrayObject);

	glGenBuffers(1, &((fireWood)->vertexBufferObject));
	glBindBuffer(GL_ARRAY_BUFFER, (fireWood)->vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, fire_NVertices *8* sizeof(float), fire_Vertices, GL_STATIC_DRAW);
	CHECK_GL_ERROR();

	glGenBuffers(1, &((fireWood)->elementBufferObject));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (fireWood)->elementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, fire_NTriangles*3 * sizeof(unsigned int), fire_Triangles, GL_STATIC_DRAW);
	CHECK_GL_ERROR();

	glEnableVertexAttribArray(shaderProgram->locations.position);
	CHECK_GL_ERROR();
	glVertexAttribPointer(shaderProgram->locations.position, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);

	CHECK_GL_ERROR();

	glEnableVertexAttribArray(shaderProgram->locations.normal);
	CHECK_GL_ERROR();
	glVertexAttribPointer(shaderProgram->locations.normal, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(2 * 3 * sizeof(float)));

	fireWoodLocation = localModelMatrix;
	fireWoodLocation = glm::scale(fireWoodLocation, FIRE_SMALL_SCALE);
	fireWoodLocation = glm::translate(fireWoodLocation, FIRE_SMALL_TRANSLATE);
	fireWoodLocation = glm::rotate(fireWoodLocation, -1.6f,FIRE_ROTATE);
	(fireWood)->ambient = glm::vec3(0.18f, 0.13f, 0.10f);
	(fireWood)->diffuse = glm::vec3(0.18f, 0.13f, 0.10f);
	(fireWood)->specular = glm::vec3(0.0f, 0.0f, 0.0f);
	(fireWood)->shininess = 0.0f;
	(fireWood)->texture = 0;
	(fireWood)->numTriangles = fire_NTriangles;
	glBindVertexArray(0);
	CHECK_GL_ERROR();
}

Fire::~Fire() {
	for (auto geometry : geometries) {
		glDeleteVertexArrays(1, &(geometry->vertexArrayObject));
		glDeleteBuffers(1, &(geometry->elementBufferObject));
		glDeleteBuffers(1, &(geometry->vertexBufferObject));
		delete geometry;
	}
	initialized = false;
}
