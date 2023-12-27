#pragma once

#include "object.h"
#include <vector>

class Fire : public ObjectInstance
{
public:

	Fire(ShaderProgram* shdrPrg, FireShaderProgram* fireShader);
	~Fire();

	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;
	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) override;
	void draw_flames(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);
	void add_wood();
	void pop_smoke(int index);
	void init_fireWood();
	void draw_fireWood(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);
	void update_fire_size();

private:

	bool initialized;  ///< object has the shader with defined locations
};

