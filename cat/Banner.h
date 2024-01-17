#pragma once
#pragma once

#include "object.h"
#include <vector>

class Banner : public ObjectInstance
{
public:

	Banner(ShaderProgram* shdrPrg);
	~Banner();

	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;
	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) override;
	//void draw_texture(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);


private:

	bool initialized;  ///< object has the shader with defined locations
	glm::mat4 location;
};
