#include <iostream>
#include "Banner.h"
//BannerShaderProgram* BannerShdr;

void Banner::update(float elapsedTime, const glm::mat4* parentModelMatrix) {
	currentTime = elapsedTime;
	ObjectInstance::update(elapsedTime, parentModelMatrix);
}

void Banner::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_DEPTH_TEST);
	glUseProgram(shaderProgram->program);
	for (auto geometry : geometries) {		

		//glm::mat4 matrix = glm::translate(glm::mat4(1.0f), position);
		//matrix = glm::scale(matrix, glm::vec3(size));

		//glm::mat4 PVMmatrix = projectionMatrix * viewMatrix * matrix;

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

		//matrix = matrix * billboardRotationMatrix; // make billboard to face the camera
		glm::mat4 PVMmatrix = projectionMatrix * viewMatrix * matrix;
		glUniformMatrix4fv(shaderProgram->locations.PVMmatrix, 1, GL_FALSE, glm::value_ptr(PVMmatrix));        // model-view-projection
		glUniform1f(shaderProgram->locations.time, currentTime - startTime);
		glUniform1i(shaderProgram->locations.texSampler, 0);

		glBindTexture(GL_TEXTURE_2D, geometry->texture);
		glBindVertexArray(geometry->vertexArrayObject);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, geometry->numTriangles);

		CHECK_GL_ERROR();

		glBindVertexArray(0);
		glUseProgram(0);

		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
	}
	
	
}

Banner::Banner(ShaderProgram* shdrPrg) : ObjectInstance(shdrPrg), initialized(false)
{
	ObjectGeometry* geometry = new ObjectGeometry;

	(geometry)->texture = pgr::createTexture(BANNER_TEXTURE_NAME);
	glBindTexture(GL_TEXTURE_2D, (geometry)->texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);

	glGenVertexArrays(1, &((geometry)->vertexArrayObject));
	glBindVertexArray((geometry)->vertexArrayObject);

	glGenBuffers(1, &((geometry)->vertexBufferObject));
	glBindBuffer(GL_ARRAY_BUFFER, (geometry)->vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bannerVertexData), bannerVertexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(shdrPrg->locations.position);
	glEnableVertexAttribArray(shdrPrg->locations.texCoord);
	// vertices of triangles - start at the beginning of the interlaced array
	glVertexAttribPointer(shdrPrg->locations.position, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	// texture coordinates of each vertices are stored just after its position
	glVertexAttribPointer(shdrPrg->locations.texCoord, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindVertexArray(0);

	(geometry)->numTriangles = bannerNumQuadVertices;
	geometries.push_back(geometry);
	initialized = true;
}
Banner::~Banner() {
	for (auto geometry : geometries) {
		glDeleteVertexArrays(1, &(geometry->vertexArrayObject));
		glDeleteBuffers(1, &(geometry->elementBufferObject));
		glDeleteBuffers(1, &(geometry->vertexBufferObject));
		delete geometry;
	}

	initialized = false;
}