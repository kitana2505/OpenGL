#pragma once

#include "object.h"
#include "pgr.h"

class Teapot : public ObjectInstance
{
public:

	Teapot(ShaderProgram* shdrPrg = nullptr);
	~Teapot();

	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;
	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) override;

private:

	bool initialized;  ///< object has the shader with defined locations
	const pgr::MeshData& mesh = pgr::teapotData;
};


