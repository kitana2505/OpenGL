#pragma once
extern const int PoleTrianglesCount;
extern const float invSqrt2;
//extern const float PoleVertices;

#include "object.h"

class Pole : public ObjectInstance
{
public:

	Pole(ShaderProgram* shdrPrg);
	~Pole();

	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;
	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) override;
	glm::mat4 alignObject(const glm::vec3& position, const glm::vec3& front, const glm::vec3& up);

	static Pole* createPole(ShaderProgram* shdrPrg, const glm::vec3& PolePosition, float elapsedTime); // dont need to create Pole instance
	// should return the created Pole return
private:

	bool initialized;  ///< object has the shader with defined locations
	std::vector<glm::mat4> locations;
	bool useLighting = true;
};
