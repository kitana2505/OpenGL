#pragma once


#include "object.h"

class Rabbit : public ObjectInstance
{
public:

	Rabbit(ShaderProgram* shdrPrg = nullptr);
	~Rabbit();

	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;
	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) override;

private:

	bool initialized;  
	std::vector<glm::mat4> locations;
};

