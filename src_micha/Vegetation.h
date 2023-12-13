#pragma once

#include "object.h"

class Vegetation : public ObjectInstance
{
public:

	Vegetation(const char* obj_name, float spacing, glm::vec3 rotation, float rotation_size, glm::vec3 translation, float scale, ShaderProgram* shdrPrg);
	~Vegetation();

	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;
	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) override;

private:

	bool initialized;  ///< object has the shader with defined locations
	std::vector<glm::mat4> locations;
};

