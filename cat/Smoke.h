#pragma once

#include "object.h"

class Smoke : public ObjectInstance
{
public:
	FireShaderProgram* shdr;
	Smoke(FireShaderProgram* shdrPrg, int*currentfiresize);
	~Smoke();

	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;
	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) override;
	void pop_smoke(int index);
	void draw_smoke(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix,ObjectGeometry geometry, glm::mat4 location);

private:

	bool initialized;  ///< object has the shader with defined locations
};

