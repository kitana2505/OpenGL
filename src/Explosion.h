#pragma once
extern const int missileTrianglesCount;
extern const float invSqrt2;
//extern const float missileVertices;

#include "object.h"

class Explosion : public ObjectInstance
{
public:
	Explosion(ShaderProgram* shdrPrg, ExplosionShaderProgram* explosionShaderProgram);
	~Explosion();


	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;
	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) override;
private:
	bool initialized;  ///< object has the shader with defined locations
	std::vector<glm::mat4> locations;
	ObjectGeometry* geometry;
};