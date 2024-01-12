#include <iostream>
#include "Animal_Turtle.h"
/// Number of control points of the animation curve.
const size_t  curveSize = 12;
/// Control points of the animation curve.
/*glm::vec3 curveData[] = {
  glm::vec3(0.00, 0.0,  0.0),

  glm::vec3(-0.33,  0.35, 0.0),
  glm::vec3(-0.66,  0.35, 0.0),
  glm::vec3(-1.00,  0.0, 0.0),
  glm::vec3(-0.66, -0.35, 0.0),
  glm::vec3(-0.33, -0.35, 0.0),

  glm::vec3(0.00,  0.0, 0.0),

  glm::vec3(0.33,  0.35, 0.0),
  glm::vec3(0.66,  0.35, 0.0),
  glm::vec3(1.00,  0.0, 0.0),
  glm::vec3(0.66, -0.35, 0.0),
  glm::vec3(0.33, -0.35, 0.0)
};*/

glm::vec3 curveData[] = {
  glm::vec3(0.00, 0.0,  0.0),

  glm::vec3(-0.33,  0.0, 0.35),
  glm::vec3(-0.66,  0.0, 0.35),
  glm::vec3(-1.00,  0.0, 0.0),
  glm::vec3(-0.66, 0.0, -0.35),
  glm::vec3(-0.33, 0.0, -0.35),

  glm::vec3(0.00,  0.0, 0.0),

  glm::vec3(0.33,  0.0, 0.35),
  glm::vec3(0.66,  0.0, 0.35),
  glm::vec3(1.00,  0.0, 0.0),
  glm::vec3(0.66, 0.0, -0.35),
  glm::vec3(0.33, 0.0, -0.35)
};

void Turtle::update(float elapsedTime, const glm::mat4* parentModelMatrix) {

	
	currentTime = elapsedTime;
			//TODO: where startTime defined?
	float curveParamT = speed * (currentTime - startTime);

	position = initPosition + evaluateClosedCurve(curveData, curveSize, curveParamT) * glm::vec3(10);
	direction = glm::normalize(evaluateClosedCurve_1stDerivative(curveData, curveSize, curveParamT));


	localModelMatrix = glm::translate(glm::mat4(1.0f), position);
	ObjectInstance::update(elapsedTime, parentModelMatrix);
}

void Turtle::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
	if (initialized && (shaderProgram != nullptr)) {
		glUseProgram(shaderProgram->program);
		for (auto geometry : geometries) {
			//for (auto location : locations) {
			glm::mat4 modelMatrix = alignObject(position,direction, glm::vec3(0.0f, 0.0f, 1.0f));
				glBindVertexArray(geometry->vertexArrayObject);
				setTransformUniforms(*shaderProgram, location * localModelMatrix, viewMatrix, projectionMatrix);
				//setTransformUniforms(*shaderProgram, localModelMatrix, viewMatrix, projectionMatrix);
				setMaterialUniforms(
					*shaderProgram,
					geometry->ambient,
					geometry->diffuse,
					geometry->specular,
					geometry->shininess,
					geometry->texture
				);
				glDrawElements(GL_TRIANGLES, geometry->numTriangles * 3, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);


			//}
		}
		glUseProgram(0);

	}
	else {
		//std::cout << "Can't draw Turtle: triangle not initialized properly!" << std::endl;
	}
}

Turtle::Turtle(ShaderProgram* shdrPrg) : ObjectInstance(shdrPrg), initialized(false)
{
	//this->shaderProgram = shdrPrg;
	if (loadMultipleMeshes(TURTLE_MODEL, shdrPrg, geometries) != true) {
		std::cerr << "initializeModels(): Turtle model loading failed." << std::endl;
	}
	else {
		//geometries[0]->texture= pgr::createTexture("\data\Spruce_obj");;
		initialized = true;
	}
	position = TURTLE_INITIAL_POS;
	size = TURTLE_SCALE;
	speed = TURTLE_SPEED;
	initPosition = TURTLE_INITIAL_POS;
	startTime = 0.0f;
	location = glm::scale(globalModelMatrix, glm::vec3(size));
	location = glm::translate(location, position);
	location = glm::rotate(location, TURTLE_ROTATION, glm::vec3(0, 1, 0));
	

	CHECK_GL_ERROR();

}
Turtle::~Turtle() {
	for (auto geometry : geometries) {
		glDeleteVertexArrays(1, &(geometry->vertexArrayObject));
		glDeleteBuffers(1, &(geometry->elementBufferObject));
		glDeleteBuffers(1, &(geometry->vertexBufferObject));
		delete geometry;
	}

	initialized = false;
}


glm::vec3 Turtle::evaluateCurveSegment(
	const glm::vec3& P0,
	const glm::vec3& P1,
	const glm::vec3& P2,
	const glm::vec3& P3,
	const float t
) {
	glm::vec3 result(0.0, 0.0, 0.0);

	// evaluate point on a curve segment (defined by the control points P0...P3)
	// for the given value of parameter t

	const float t2 = t * t;
	const float t3 = t * t2;

	result = P0 * (-t3 + 2.0f * t2 - t)
	+ P1 * (3.0f * t3 - 5.0f * t2 + 2.0f)
	+ P2 * (-3.0f * t3 + 4.0f * t2 + t)
	+ P3 * (t3 - t2);

	result *= 0.5f;

	return result;
}

glm::vec3 Turtle::evaluateCurveSegment_1stDerivative(
	const glm::vec3& P0,
	const glm::vec3& P1,
	const glm::vec3& P2,
	const glm::vec3& P3,
	const float t
) {
	glm::vec3 result(1.0, 0.0, 0.0);

	// evaluate first derivative for a point on a curve segment (defined by the control points P0...P3)
	// for the given value of parameter t

	const float t2 = t * t;

	result = P0 * (-3.0f * t2 + 4.0f * t - 1.0f)
		+ P1 * (9.0f * t2 - 10.0f * t)
		+ P2 * (-9.0f * t2 + 8.0f * t + 1.0f)
		+ P3 * (3.0f * t2 - 2.0f * t);

	result *= 0.5f;

	return result;
}

glm::vec3 Turtle::evaluateClosedCurve(
	const glm::vec3 points[],
	const size_t    count,
	const float     t
) {
	glm::vec3 result(0.0, 0.0, 0.0);

	// based on the value of parameter t first find corresponding segment and its control points => i
	// and then call evaluateCurveSegment function with proper parameters to get a point on a closed curve

	float param = cyclic_clamp(t, 0.0f, float(count));
	size_t index = size_t(param);

	result = evaluateCurveSegment(
		points[(index - 1 + count) % count],
		points[(index) % count],
		points[(index + 1) % count],
		points[(index + 2) % count],
		param - floor(param)
	);

	return result;
}

glm::vec3 Turtle::evaluateClosedCurve_1stDerivative(
	const glm::vec3 points[],
	const size_t    count,
	const float     t
) {
	glm::vec3 result(1.0, 0.0, 0.0);

	// based on the value of parameter t first find corresponding curve segment and its control points => i
	// and then call evaluateCurveSegment_1stDerivative function with proper parameters
	// to get a derivative for the given point on a closed curve

	float param = cyclic_clamp(t, 0.0f, float(count));
	size_t index = size_t(param);

	result = evaluateCurveSegment_1stDerivative(
		points[(index - 1 + count) % count],
		points[(index) % count],
		points[(index + 1) % count],
		points[(index + 2) % count],
		param - floor(param)
	);

	return result;
}

glm::mat4 Turtle::alignObject(const glm::vec3& position, const glm::vec3& front, const glm::vec3& up) {

	glm::vec3 z = -glm::normalize(front);

	if (isVectorNull(z))
		z = glm::vec3(0.0, 0.0, 1.0);

	glm::vec3 x = glm::normalize(glm::cross(up, z));

	if (isVectorNull(x))
		x = glm::vec3(1.0, 0.0, 0.0);

	glm::vec3 y = glm::cross(z, x);
	//mat4 matrix = mat4(1.0f);
	glm::mat4 matrix = glm::mat4(
		x.x, x.y, x.z, 0.0,
		y.x, y.y, y.z, 0.0,
		z.x, z.y, z.z, 0.0,
		position.x, position.y, position.z, 1.0
	);

	return matrix;
}





template <typename T>
T Turtle::cyclic_clamp(const T value, const T minBound, const T maxBound) {

	T amp = maxBound - minBound;
	T val = fmod(value - minBound, amp);

	if (val < T(0))
		val += amp;

	return val + minBound;
}

bool Turtle::isVectorNull(const glm::vec3& vect) {

	return !vect.x && !vect.y && !vect.z;
}