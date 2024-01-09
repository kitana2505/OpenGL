#pragma once
extern const int missileTrianglesCount;
extern const float invSqrt2;
//extern const float missileVertices;

#include "object.h"

class Missile : public ObjectInstance
{
public:

	Missile(ShaderProgram* shdrPrg);
	~Missile();

	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;
	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) override;
	static Missile* createMissile(ShaderProgram* shdrPrg, const glm::vec3& missilePosition, const glm::vec3& missileDirection, float& missileLaunchTime, float elapsedTime); // dont need to create Missile instance
	// should return the created Missile return
private:

	bool initialized;  ///< object has the shader with defined locations
	std::vector<glm::mat4> locations;
	bool useLighting = false;
};
