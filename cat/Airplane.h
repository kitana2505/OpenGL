#pragma once

#include "object.h"

class Airplane : public ObjectInstance
{
public:

	Airplane(ShaderProgram* shdrPrg = nullptr);
	~Airplane();

	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;
	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) override;
	glm::vec3 evaluateCurveSegment(
		const glm::vec3& P0,
		const glm::vec3& P1,
		const glm::vec3& P2,
		const glm::vec3& P3,
		const float t
	);
	glm::vec3 evaluateCurveSegment_1stDerivative(
		const glm::vec3& P0,
		const glm::vec3& P1,
		const glm::vec3& P2,
		const glm::vec3& P3,
		const float t
	);
	glm::vec3 evaluateClosedCurve(
		const glm::vec3 points[],
		const size_t    count,
		const float     t
	);
	glm::vec3 evaluateClosedCurve_1stDerivative(
		const glm::vec3 points[],
		const size_t    count,
		const float     t
	);
	glm::mat4 alignObject(const glm::vec3& position, const glm::vec3& front, const glm::vec3& up);
	bool isVectorNull(const glm::vec3& vect);
	template <typename T>
	T cyclic_clamp(const T value, const T minBound, const T maxBound);

private:

	bool initialized;  ///< object has the shader with defined loAirplaneions
	//std::vector<glm::mat4> locations;
	glm::mat4  location;
	glm::vec3 initPosition;
};



