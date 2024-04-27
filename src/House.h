#pragma once

#include "object.h"

class House : public ObjectInstance
{
public:

	House(ShaderProgram* shdrPrg = nullptr);
	~House();

	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;
	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) override;

private:

	bool initialized;  ///< object has the shader with defined locations
	std::vector<glm::mat4> locations;
};


