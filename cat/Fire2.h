#pragma once
#pragma once

#include "object.h"
#include <vector>

class Fire2 : public ObjectInstance
{
public:

	Fire2(ShaderProgram* shdrPrg, FireShaderProgram* fireShader);
	~Fire2();

	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;
	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) override;
	void draw_flames(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);
	

private:

	bool initialized;  ///< object has the shader with defined locations
};

