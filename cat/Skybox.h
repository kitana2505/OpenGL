#pragma once

#include "object.h"

class Skybox : public ObjectInstance
{
public:
	SkyboxShaderProgram *shaderProgram;
	Skybox(SkyboxShaderProgram* shdrPrg);
	void load_skybox(const char* prefix, const char* suffixes[6]);
	~Skybox();

	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;
	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) override;
	const char* day_suffixes[6]= { "front", "back", "down", "up", "right", "left" };
	const char* night_suffixes[6] = { "posx", "negx", "posy", "negy", "posz", "negz" };

private:

	bool initialized;  
};

