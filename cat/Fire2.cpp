#include <iostream>
#include "Fire2.h"


ObjectGeometry* flames2 = new ObjectGeometry;
ObjectGeometry* fireWood2 = new ObjectGeometry;
FireShaderProgram* fireShader2;
float elapsed_time2 = 0.0f;
glm::mat4 fireWoodLocation2;
float firesize2 = 10;
int current_fire_size2 = 4;

void Fire2::update(float currentTime, const glm::mat4* parentModelMatrix) {
	elapsed_time2 += currentTime;
	ObjectInstance::update(currentTime, parentModelMatrix);
}


void Fire2::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	if (initialized && (shaderProgram != nullptr)) {
		glUseProgram(shaderProgram->program);
		glm::vec3 firelocation = glm::vec3((viewMatrix * glm::vec4(glm::vec3(0.0f, 0.0f, 0.0f), 1.0f)));
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
		draw_flames(viewMatrix, projectionMatrix);
		glDisable(GL_STENCIL_TEST);
		ObjectInstance::draw(viewMatrix, projectionMatrix);
	}
	else {
		std::cout << "Can't draw Fire: Fire not initialized properly!" << std::endl;
	}
}
void Fire2::draw_flames(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{	//draw fire at last
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	// ========  END OF SOLUTION - TASK 4_2-2  ======== //

	glUseProgram(fireShader2->program);

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
	matrix = glm::translate(matrix, glm::vec3(0, 1.f, 0));

	//glm::mat4 matrix = globalModelMatrix;
	matrix = matrix * billboardRotationMatrix; // make billboard to face the camera
	glm::mat4 PVMmatrix = projectionMatrix * viewMatrix * matrix;

	glUniformMatrix4fv(fireShader2->VmatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));   // view

	glUniformMatrix4fv(fireShader2->PVMmatrixLocation, 1, GL_FALSE, glm::value_ptr(PVMmatrix));  // model-view-projection
	glUniform1f(fireShader2->timeLocation, elapsed_time2);
	glUniform1i(fireShader2->texSamplerLocation, 0);
	glUniform2i(fireShader2->frames, 3, 2);
	glUniform1f(fireShader2->frameDurationLocation, 0.5f);
	glUniform1f(fireShader2->scale, 1.0f);

	glBindVertexArray(flames2->vertexArrayObject);
	glBindTexture(GL_TEXTURE_2D, flames2->texture);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, flames2->numTriangles);

	glBindVertexArray(0);
	glUseProgram(0);

	glDisable(GL_BLEND);

	return;
}


Fire2::Fire2(ShaderProgram* shdrPrg, FireShaderProgram* fireShaderProgram)
{
	this->shaderProgram = shdrPrg;
	fireShader2 = fireShaderProgram;
	if (loadMultipleMeshes(FIRE_MODEL_NAME2, shdrPrg, geometries) != true) {
		std::cerr << "initializeModels(): Fire model loading failed." << std::endl;
	}
	else {
		initialized = true;
		this->localModelMatrix = glm::scale(localModelMatrix, FIRE_SCALE);
		this->localModelMatrix = glm::translate(localModelMatrix, glm::vec3(FIRE_TRANSLATE));
		this->size = FIRE_SIZE;
		glUseProgram(shaderProgram->program);
		glUniform3f(shaderProgram->locations.firePosition, 0, 0, 0);
		glUniform1f(shaderProgram->locations.fireStrength, 10);
		//glUniform1f(shaderProgram->locations.fireFallof, 1.5f);
		glUseProgram(0);

	}

	//initialize flames2
	flames2->texture = pgr::createTexture(FIRE_TEXTURE_NAME);
	CHECK_GL_ERROR();
	glGenVertexArrays(1, &(flames2->vertexArrayObject));
	glBindVertexArray(flames2->vertexArrayObject);

	glGenBuffers(1, &(flames2->vertexBufferObject)); \
		glBindBuffer(GL_ARRAY_BUFFER, flames2->vertexBufferObject);
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

	flames2->numTriangles = flameNumQuadVertices;

}



Fire2::~Fire2() {
	for (auto geometry : geometries) {
		glDeleteVertexArrays(1, &(geometry->vertexArrayObject));
		glDeleteBuffers(1, &(geometry->elementBufferObject));
		glDeleteBuffers(1, &(geometry->vertexBufferObject));
		delete geometry;
	}
	initialized = false;
}
