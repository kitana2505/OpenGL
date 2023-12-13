#pragma once

#include "object.h"
#include <list>
#include <vector>

class Firewood : public ObjectInstance
{
public:

	Firewood(const char* obj_name, float spacing, glm::vec3 rotation, float rotation_size, glm::vec3 translation, float scale, ShaderProgram* shdrPrg);
	~Firewood();

	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;
	void collect_wood(int index);
	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) override;

private:

	bool initialized;  ///< object has the shader with defined locations
	std::list<glm::mat4> locations;
};

