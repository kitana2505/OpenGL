	//----------------------------------------------------------------------------------------
/**
 * \file    skeleton.cpp : This file contains the 'main' function and callbacks.
			Program execution begins and ends there.
 * \author  Jaroslav Sloup, Petr Felkel
 * \date    2022/03/03
 * \brief   Prepared for the Computer graphics course on FEE and FIT CTU in Prague CZ
 */

 /**
  * \brief	\mainpage Documentation of the skeleton for the Computer graphics course on FEE and FIT CTU CZ
  *
  * This is the skeleton of an OpenGL application written in FreeGLUT and PGR framework.
  * It serves as a starter boiler-plate code for a semester project - a simple interactive application.
  * Write your code into the functions prepared. Add your data (models, textures, ...) and logic.
  *
  *
  * Comment your code using the [doxygen](https://www.doxygen.nl/index.html) documenting system style.
  * Create "doxygen" directory, make it current by "cd doxygen", prepare a configuration file with "doxygen -g" and edit the details.
  *
  * Start by renaming of this file from skeleton.cpp to <your_name>.cpp and the project to <your_name>.vcxproj
  *
  * In <your_name>.vcxproj:
  *   - rename skeleton in <RootNamespace>skeleton</RootNamespace> to <your_name>
  *   - replace skeleton.cpp in <ClCompile Include="skeleton.cpp" /> to <your_name>.cpp
  *
  * Start coding and documenting. Generate the documentation by the command "doxygen" in the "doxygen" directory.
  *
  */


  // TODO: tabulka klaves a jeji obsluha keyPressed/keyReleased a timer


#include <iostream>
#include <list>
#include "data.h"
#include "pgr.h"
#include "object.h"
//#include "triangle.h"
//#include "singlemesh.h"
#include "Fire.h"
#include "Fire2.h"
#include "Tree.h"
#include "House.h"
#include "Ground.h"
#include "Skybox.h"
#include "Animal_cat.h"
#include "Animal_turtle.h"
#include "Missile.h"

//constexpr int WINDOW_WIDTH = 500;
//constexpr int WINDOW_HEIGHT = 500;
// constexpr char WINDOW_TITLE[] = "PGR: Application Skeleton";

ObjectList objects;
// shared shader programs
ShaderProgram commonShaderProgram;
FireShaderProgram fireShaderProgram;
SkyboxShaderProgram skyboxShaderProgram;
MissileShaderProgram missileShaderProgram;
ObjectList missleList;


// -----------------------  OpenGL stuff ---------------------------------

struct _GameState {
	int windowWidth;
	int windowHeight;

	/// when last update happened
	float last_update;
	glm::vec3 player_position;
	glm::vec3 player_direction;

	/// which camera is active
	int camera_index = 0;
	float cameraElevationAngle;
	float cameraRotationAngle;

	/// when changing camera view, I need to know, in which point I am starting to make it smooth
	glm::vec3 initial_camera_position;

	///current camera position
	glm::vec3 camera_position;

	/// where the camera should be moved
	glm::vec3 target_camera_position;

	/// whether camera is in correct place or should be moved
	bool move_camera;
	bool keyMap[KEYS_COUNT];
	bool reflectorOn;

	/// Sunlight should be on/off
	bool sunOn;
	 Fire* fire;
	 Fire2* fire2;
	// Firewood* firewood;
	 Skybox* skybox;

	/// number of wood stacks in inventory
	//int wood_in_inventory = 0;
	 float sunStrength;


	 float elapsedTime;
	 float missileLaunchTime;
	 float ufoMissileLaunchTime;
	 Missile* missile;
	 bool launchMissile;

}gameState;


bool check_bounds(glm::vec3 position) {
	if (abs(position.x) > SCENE_WIDTH) {
		return false;
	}
	if (abs(position.z) > SCENE_DEPTH) {
		return false;
	}
	return true;
}

void move_player(float deltaTime) {
	int forward = 0;
	int sideways = 0;

	if (gameState.keyMap[FORWARD]) {
		forward += 1;
		sideways += 0;
	}
	if (gameState.keyMap[BACKWARD]) {
		forward += -1;
		sideways += 0;
	}

	if (gameState.keyMap[LEFT]) {
		forward += 0;
		sideways += -1;
	}

	if (gameState.keyMap[RIGHT]) {
		forward += 0;
		sideways += 1;
	}

	float x = cos(glm::radians(gameState.cameraRotationAngle)) * forward + sin(glm::radians(gameState.cameraRotationAngle)) * -sideways;
	float z = cos(glm::radians(gameState.cameraRotationAngle)) * sideways + sin(glm::radians(gameState.cameraRotationAngle)) * forward;
	gameState.player_direction = glm::vec3(x, 0, z);
	if (x != 0 && z != 0) {
		gameState.player_direction = glm::normalize(gameState.player_direction);
	}

	//gameState.player_direction = glm::normalize(glm::vec3(0, 0,0));
	if (gameState.keyMap[RUN]) {
		if (check_bounds(gameState.player_position + gameState.player_direction * PLAYER_RUNNING_SPEED * deltaTime)) {
			gameState.player_position += gameState.player_direction * PLAYER_RUNNING_SPEED * deltaTime;
		}

	}
	else {
		if (check_bounds(gameState.player_position + gameState.player_direction * PLAYER_WALKING_SPEED * deltaTime)) {
			gameState.player_position += gameState.player_direction * PLAYER_WALKING_SPEED * deltaTime;
		}
	}

}

void shooting(ObjectList objects, float elapsedTime)
{
	//if (gameState.keyMap[KEY_SPACE] == true) {
		// missile position and direction
	glm::vec3 missilePosition = objects[3]->position;
	glm::vec3 missileDirection = objects[3]->direction;

	//missilePosition += missileDirection * 1.5f * CAT_SCALE;
	missilePosition += missileDirection  * CAT_SCALE;
	//MissileShaderProgram* missileShader = new MissileShaderProgram;
	Missile* newMissile = Missile::createMissile(&commonShaderProgram, missilePosition, missileDirection, gameState.missileLaunchTime,gameState.elapsedTime);
	//}

	// test collisions among objects in the scene
	//checkCollisions();
	//newMissile->draw();
	missleList.push_back(newMissile);
	gameState.launchMissile = false;
}
const std::string skyboxVShader(
	"#version 140\n"
	"\n"
	"uniform mat4 inversePVmatrix;\n"
	"in vec2 screenCoord;\n"
	"out vec3 texCoord_v;\n"
	"\n"
	"void main() {\n"
	"  vec4 farplaneCoord = vec4(screenCoord, 0.9999, 1.0);\n"
	"  vec4 worldViewCoord = inversePVmatrix * farplaneCoord;\n"
	"  texCoord_v = - worldViewCoord.xyz / worldViewCoord.w;\n"
	"  gl_Position = farplaneCoord;\n"
	"}\n"
);

const std::string skyboxFShader(
	"#version 140\n"
	"\n"
	"uniform samplerCube skyboxSampler;\n"
	"in vec3 texCoord_v;\n"
	"out vec4 color_f;\n"
	"\n"
	"void main() {\n"
	"  color_f = texture(skyboxSampler, texCoord_v);\n"
	"}\n"
);

/**
 * \brief s and compile shader programs. Get attribute locations.
 */
void loadShaderPrograms() //define at least 1 shader obj
{
	//std::string vertexShaderSrc =
	//	"#version 140\n"
	//	"in vec2 position;\n"
	//	"uniform mat4 PVM;\n"
	//	"void main() {\n"
	//	"  gl_Position = PVM * vec4(position, 0.0f, 1.0f);\n" //3rd coord = 0, homogeneour = 1
	//	"}\n"
	//	;

	//std::string fragmentShaderSrc =
	//	"#version 140\n"
	//	"out vec4 fragmentColor;"
	//	"void main() {\n"
	//	"  fragmentColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
	//	"}\n"
	//	;

	GLuint shaders[] = {
	  pgr::createShaderFromFile(GL_VERTEX_SHADER, "commonShader.vert"),
	  pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "commonShader.frag"),
	  0
	};

	commonShaderProgram.program = pgr::createProgram(shaders);
	commonShaderProgram.locations.position = glGetAttribLocation(commonShaderProgram.program, "position");
	commonShaderProgram.locations.color = glGetAttribLocation(commonShaderProgram.program, "color");

	// other attributes and uniforms
	commonShaderProgram.locations.PVMmatrix = glGetUniformLocation(commonShaderProgram.program, "PVMmatrix");

	commonShaderProgram.locations.normal = glGetAttribLocation(commonShaderProgram.program, "normal");
	commonShaderProgram.locations.texCoord = glGetAttribLocation(commonShaderProgram.program, "texCoord");

	// get uniforms locations
	commonShaderProgram.locations.Vmatrix = glGetUniformLocation(commonShaderProgram.program, "Vmatrix");
	commonShaderProgram.locations.Mmatrix = glGetUniformLocation(commonShaderProgram.program, "Mmatrix");
	commonShaderProgram.locations.normalMatrix = glGetUniformLocation(commonShaderProgram.program, "normalMatrix");
	// material
	commonShaderProgram.locations.ambient = glGetUniformLocation(commonShaderProgram.program, "material.ambient");
	commonShaderProgram.locations.diffuse = glGetUniformLocation(commonShaderProgram.program, "material.diffuse");
	commonShaderProgram.locations.specular = glGetUniformLocation(commonShaderProgram.program, "material.specular");
	commonShaderProgram.locations.shininess = glGetUniformLocation(commonShaderProgram.program, "material.shininess");
	// texture
	commonShaderProgram.locations.texSampler = glGetUniformLocation(commonShaderProgram.program, "texSampler");
	commonShaderProgram.locations.useTexture = glGetUniformLocation(commonShaderProgram.program, "material.useTexture");

	// flashlight
	commonShaderProgram.locations.reflectorPosition = glGetUniformLocation(commonShaderProgram.program, "reflectorPosition");
	commonShaderProgram.locations.reflectorDirection = glGetUniformLocation(commonShaderProgram.program, "reflectorDirection");
	commonShaderProgram.locations.flashlightOn = glGetUniformLocation(commonShaderProgram.program, "reflectorOn");
	commonShaderProgram.locations.sunOn = glGetUniformLocation(commonShaderProgram.program, "sunOn");
	commonShaderProgram.locations.sunStrength = glGetUniformLocation(commonShaderProgram.program, "sunStrength");


	//fire
	commonShaderProgram.locations.firePosition = glGetUniformLocation(commonShaderProgram.program, "firePosition");
	commonShaderProgram.locations.fireStrength = glGetUniformLocation(commonShaderProgram.program, "fireStrength");
	commonShaderProgram.locations.fireFallof = glGetUniformLocation(commonShaderProgram.program, "fireFallof");
	commonShaderProgram.locations.fireDiffuse = glGetUniformLocation(commonShaderProgram.program, "fireDiffuse");
	commonShaderProgram.locations.fireSpecular = glGetUniformLocation(commonShaderProgram.program, "fireSpecular");
	commonShaderProgram.locations.fireAmbient = glGetUniformLocation(commonShaderProgram.program, "fireAmbient");

	//fog
	commonShaderProgram.locations.fogColor = glGetUniformLocation(commonShaderProgram.program, "fogColor");

	commonShaderProgram.initialized = true;

	// push vertex shader and fragment shader
	GLuint shaders2[] = {
	  pgr::createShaderFromFile(GL_VERTEX_SHADER,"fireShader.vert"),
	  pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "fireShader.frag"),
	  0
		};

	// create the program with two shaders
	fireShaderProgram.program = pgr::createProgram(shaders2);

	// get position and texture coordinates attributes locations
	fireShaderProgram.posLocation = glGetAttribLocation(fireShaderProgram.program, "position");
	fireShaderProgram.texCoordLocation = glGetAttribLocation(fireShaderProgram.program, "texCoord");
	// get uniforms locations
	fireShaderProgram.texCoordLocation = 1;
	fireShaderProgram.PVMmatrixLocation = glGetUniformLocation(fireShaderProgram.program, "PVMmatrix");
	fireShaderProgram.VmatrixLocation = glGetUniformLocation(fireShaderProgram.program, "Vmatrix");
	fireShaderProgram.timeLocation = glGetUniformLocation(fireShaderProgram.program, "time");
	fireShaderProgram.texSamplerLocation = glGetUniformLocation(fireShaderProgram.program, "texSampler");
	fireShaderProgram.frameDurationLocation = glGetUniformLocation(fireShaderProgram.program, "frameDuration");
	fireShaderProgram.frames = glGetUniformLocation(fireShaderProgram.program, "pattern");
	fireShaderProgram.scale = glGetUniformLocation(fireShaderProgram.program, "scale");


	GLuint shaders3[] = {
	  pgr::createShaderFromSource(GL_VERTEX_SHADER,skyboxVShader),
	  pgr::createShaderFromSource(GL_FRAGMENT_SHADER, skyboxFShader),
	  0
	};
	skyboxShaderProgram.program = pgr::createProgram(shaders3);

	skyboxShaderProgram.screenCoord = glGetAttribLocation(skyboxShaderProgram.program, "screenCoord");
	skyboxShaderProgram.Sampler = glGetUniformLocation(skyboxShaderProgram.program, "skyboxSampler");
	skyboxShaderProgram.iPVM = glGetUniformLocation(skyboxShaderProgram.program, "inversePVmatrix");




	assert(commonShaderProgram.locations.PVMmatrix != -1);
	assert(commonShaderProgram.locations.position != -1);
	// ...

}

/**
 * \brief Delete all shader program objects.
 */
void cleanupShaderPrograms(void) {

	pgr::deleteProgramAndShaders(commonShaderProgram.program);
}

glm::vec3 move_camera() {
	if (!gameState.move_camera) {
		gameState.camera_position = gameState.target_camera_position;
		return gameState.camera_position;
	}
	glm::vec3 position = gameState.initial_camera_position;
	glm::vec3 target = gameState.target_camera_position;
	glm::vec3 direction = (target - position) * 0.05f;

	float dis1 = glm::distance(gameState.camera_position, target);
	gameState.camera_position += direction;
	float dis2 = glm::distance(gameState.camera_position, target);
	if (dis1 <= dis2) {
		gameState.camera_position = target;
		gameState.move_camera = false;
	}
	return gameState.camera_position;
}

void setLights() {
	if (gameState.sunOn) {
		glUniform1f(commonShaderProgram.locations.sunStrength, 2.0f);
		glUniform1i(commonShaderProgram.locations.sunOn, 1);
	}
	else {
		glUniform1f(commonShaderProgram.locations.sunStrength, 0.7f);
		glUniform1i(commonShaderProgram.locations.sunOn, 1);
	}if (gameState.reflectorOn) {
		glUniform1i(commonShaderProgram.locations.flashlightOn, 1);
	}
	else {
		glUniform1i(commonShaderProgram.locations.flashlightOn, 0);
	}

}

/**
 * \brief Draw all scene objects.
 */
void drawScene(void)
{
	glm::mat4 orthoProjectionMatrix = glm::ortho(
		-SCENE_WIDTH, SCENE_WIDTH,
		-SCENE_HEIGHT, SCENE_HEIGHT,
		-10.0f * SCENE_DEPTH, 10.0f * SCENE_DEPTH
	);
	glm::mat4 orthoViewMatrix = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);

	glm::mat4 viewMatrix = orthoViewMatrix;
	glm::mat4 projectionMatrix = orthoProjectionMatrix;

	glm::vec3 cameraDirection = glm::vec3(cos(glm::radians(gameState.cameraRotationAngle)) * cos(glm::radians(gameState.cameraElevationAngle)), sin(glm::radians(gameState.cameraElevationAngle)), sin(glm::radians(gameState.cameraRotationAngle)) * cos(glm::radians(gameState.cameraElevationAngle)));

	switch (gameState.camera_index) {
	case 0:
		gameState.target_camera_position = gameState.player_position;
		break;
	case 1:
		gameState.target_camera_position = CAT_INITIAL_POS * glm::vec3(1.0f, 1.0f, 0.5f);

		// Fix camera in front of the cat
		gameState.cameraElevationAngle = 0.0f;
		gameState.cameraRotationAngle = 90.0f;
		break;
	case 2:
		gameState.target_camera_position = CAM_INIT_PLAYER;
		gameState.cameraElevationAngle = 0.0f;
		gameState.cameraRotationAngle = 135.0f;
		break;
	case 3:
		gameState.target_camera_position = STATIC_CAMERA_2;
		gameState.cameraElevationAngle = -45.0f;
		gameState.cameraRotationAngle = 135.0f;
		break;
	}
	glm::vec3 cameraPosition = move_camera();

	glm::vec3 cameraUpVector = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraCenter = cameraPosition + cameraDirection;

	viewMatrix = glm::lookAt(
		cameraPosition,
		cameraCenter,
		cameraUpVector
	);
	projectionMatrix = glm::perspective(glm::radians(60.0f), gameState.windowWidth / (float)gameState.windowHeight, 0.1f, 200.0f);
	
	// setup camera & projection transform
	glUseProgram(commonShaderProgram.program);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_STENCIL_BUFFER_BIT);


	setLights();
	glUniform3f(commonShaderProgram.locations.reflectorPosition, cameraPosition.x, cameraPosition.y, cameraPosition.z);
	glUniform3f(commonShaderProgram.locations.reflectorDirection, cameraDirection.x, cameraDirection.y, cameraDirection.z);
	glUseProgram(0);

	for (ObjectInstance* object : objects) {   // for (auto object : objects) {
		if (object != nullptr)
			object->draw(viewMatrix, projectionMatrix);
	}

	for (ObjectInstance* object : missleList) {   // for (auto object : objects) {
		if (object != nullptr)
			object->draw(viewMatrix, projectionMatrix);
	}
}


// -----------------------  Window callbacks ---------------------------------

/**
 * \brief Draw the window contents.
 */
void displayCb() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// draw the window contents (scene objects)
	drawScene();

	glutSwapBuffers(); // switch buffer after finish drawing, cannot be done automatically
}

/**
 * \brief Window was reshaped.
 * \param newWidth New window width
 * \param newHeight New window height
 */
void reshapeCb(int newWidth, int newHeight) {
	gameState.windowWidth = newWidth;
	gameState.windowHeight = newHeight;

	glViewport(0, 0, (GLsizei)newWidth, (GLsizei)newHeight);
	// TODO: Take new window size and update the application state,
	// window and projection.

	// glViewport(...);
};

void changeCamera() {
	gameState.move_camera = true;
	gameState.camera_index++;
	gameState.camera_index = gameState.camera_index % CAMERA_COUNT;
	gameState.initial_camera_position = gameState.camera_position;
};

// -----------------------  Keyboard ---------------------------------

/**
 * \brief Handle the key pressed event.
 * Called whenever a key on the keyboard was pressed. The key is given by the "keyPressed"
 * parameter, which is an ASCII character. It's often a good idea to have the escape key (ASCII value 27)
 * to call glutLeaveMainLoop() to exit the program.
 * \param keyPressed ASCII code of the key
 * \param mouseX mouse (cursor) X position
 * \param mouseY mouse (cursor) Y position
 */
void keyboardCb(unsigned char keyPressed, int mouseX, int mouseY) {
	switch (keyPressed) {
	case 27:
		glutLeaveMainLoop();
	case 'w':
	case 'W':
		//case GLUT_KEY_UP:
		gameState.keyMap[FORWARD] = true;
		break;
	case 'a':
	case 'A':
		//case GLUT_KEY_LEFT:
		gameState.keyMap[LEFT] = true;
		break;
	case 'd':
	case 'D':
		//case GLUT_KEY_RIGHT:
		gameState.keyMap[RIGHT] = true;
		break;
	case 's':
	case 'S':
		//case GLUT_KEY_DOWN:
		gameState.keyMap[BACKWARD] = true;
		break;
	case 'f':
	case 'F':
		//case GLUT_KEY_DOWN:
		if (gameState.reflectorOn) {
			gameState.reflectorOn = false;
		}
		else {
			gameState.reflectorOn = true;
		}
		break;
	case 'n':
	case 'N':
		case GLUT_KEY_DOWN:
		if (gameState.sunOn) {
			gameState.sunOn = false;
			gameState.skybox->load_skybox(SKYBOX_NIGHT_TEXTURE_NAME, gameState.skybox->night_suffixes);
			glUseProgram(commonShaderProgram.program);
			glUniform3f(commonShaderProgram.locations.fogColor, 0.0f, 0.0f, 0.0f);
			glUseProgram(0);

		}
		else {
			gameState.sunOn = true;
			gameState.skybox->load_skybox(SKYBOX_DAY_TEXTURE_NAME, gameState.skybox->day_suffixes);
			glUseProgram(commonShaderProgram.program);
			glUniform3f(commonShaderProgram.locations.fogColor, 0.5f, 0.5f, 0.5f);
			glUseProgram(0);
		}
		break;

	case 'c':
	case 'C':
		//case GLUT_KEY_DOWN:
		changeCamera();
		break;

	case ' ': // launch missile
		//if (gameState.gameOver != true)
		gameState.keyMap[KEY_SPACE] = true;
		//gameState.launchMissile = true;
		break;
	}
}
// Called whenever a key on the keyboard was released. The key is given by
// the "keyReleased" parameter, which is in ASCII. 
/**
 * \brief Handle the key released event.
 * \param keyReleased ASCII code of the released key
 * \param mouseX mouse (cursor) X position
 * \param mouseY mouse (cursor) Y position
 */
void keyboardUpCb(unsigned char keyReleased, int mouseX, int mouseY) {
	switch (keyReleased) {
	case 'w':
	case 'W':
		//case GLUT_KEY_UP:
		gameState.keyMap[FORWARD] = false;
		break;
	case 'a':
	case 'A':
		//case GLUT_KEY_LEFT:
		gameState.keyMap[LEFT] = false;
		break;
	case 'd':
	case 'D':
		//case GLUT_KEY_RIGHT:
		gameState.keyMap[RIGHT] = false;
		break;
	case 's':
	case 'S':
		//case GLUT_KEY_DOWN:
		gameState.keyMap[BACKWARD] = false;
		break;
	}
}
//
/**
 * \brief Handle the non-ASCII key pressed event (such as arrows or F1).
 *  The special keyboard callback is triggered when keyboard function (Fx) or directional
 *  keys are pressed.
 * \param specKeyPressed int value of a predefined glut constant such as GLUT_KEY_UP
 * \param mouseX mouse (cursor) X position
 * \param mouseY mouse (cursor) Y position
 */
void specialKeyboardCb(int specKeyPressed, int mouseX, int mouseY) {
	switch (specKeyPressed) {
	case GLUT_KEY_UP:
		//case GLUT_KEY_UP:
		gameState.keyMap[FORWARD] = true;
		break;
	case GLUT_KEY_LEFT:
		//case GLUT_KEY_LEFT:
		gameState.keyMap[LEFT] = true;
		break;
	case GLUT_KEY_RIGHT:
		//case GLUT_KEY_RIGHT:
		gameState.keyMap[RIGHT] = true;
		break;
	case GLUT_KEY_DOWN:
		//case GLUT_KEY_DOWN:
		gameState.keyMap[BACKWARD] = true;
		break;
	case GLUT_KEY_SHIFT_L:
		gameState.keyMap[RUN] = true;
		break;
	}
}

void specialKeyboardUpCb(int specKeyReleased, int mouseX, int mouseY) {
	switch (specKeyReleased) {
	case GLUT_KEY_UP:
		//case GLUT_KEY_UP:
		gameState.keyMap[FORWARD] = false;
		break;
	case GLUT_KEY_LEFT:
		//case GLUT_KEY_LEFT:
		gameState.keyMap[LEFT] = false;
		break;
	case GLUT_KEY_RIGHT:
		//case GLUT_KEY_RIGHT:
		gameState.keyMap[RIGHT] = false;
		break;
	case GLUT_KEY_DOWN:
		//case GLUT_KEY_DOWN:
		gameState.keyMap[BACKWARD] = false;
		break;
	case GLUT_KEY_SHIFT_L:
		gameState.keyMap[RUN] = false;
		break;
	}
} // key released

// -----------------------  Mouse ---------------------------------
// three events - mouse click, mouse drag, and mouse move with no button pressed

// 
/**
 * \brief React to mouse button press and release (mouse click).
 * When the user presses and releases mouse buttons in the window, each press
 * and each release generates a mouse callback (including release after dragging).
 *
 * \param buttonPressed button code (GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, or GLUT_RIGHT_BUTTON)
 * \param buttonState GLUT_DOWN when pressed, GLUT_UP when released
 * \param mouseX mouse (cursor) X position
 * \param mouseY mouse (cursor) Y position
 */
void mouseCb(int buttonPressed, int buttonState, int mouseX, int mouseY) {
	unsigned char objectID = 0;
	glReadPixels(mouseX, glutGet(GLUT_WINDOW_HEIGHT) - mouseY - 1, 1, 1,
		GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, &objectID
	);
	if (buttonState == 1) {
		return;
	}
}

/**
 * \brief Handle mouse dragging (mouse move with any button pressed).
 *        This event follows the glutMouseFunc(mouseCb) event.
 * \param mouseX mouse (cursor) X position
 * \param mouseY mouse (cursor) Y position
 */
float angleToBounds(float angle) {
	while (angle < 0) {
		angle += 360;
	}
	while (angle > 360) {
		angle -= 360;
	}
	return angle;
}
void mouseMotionCb(int mouseX, int mouseY) {
	if (mouseY != gameState.windowHeight / 2) {

		float cameraElevationAngleDelta = -0.2f * (mouseY - gameState.windowHeight / 2);

		if (fabs(gameState.cameraElevationAngle + cameraElevationAngleDelta) < CAMERA_ELEVATION_MAX)
			gameState.cameraElevationAngle += cameraElevationAngleDelta;

		// set mouse pointer to the window center
		glutWarpPointer(gameState.windowWidth / 2, gameState.windowHeight / 2);

		glutPostRedisplay();
	}
	if (mouseX != gameState.windowWidth / 2) {

		float cameraRotationAngle = 0.2f * (mouseX - gameState.windowWidth / 2);

		gameState.cameraRotationAngle += cameraRotationAngle;
		gameState.cameraRotationAngle = angleToBounds(gameState.cameraRotationAngle);

		// set mouse pointer to the window center
		glutWarpPointer(gameState.windowWidth / 2, gameState.windowHeight / 2);

		glutPostRedisplay();
	}
}
void passiveMouseMotionCb(int mouseX, int mouseY) {

	// mouse hovering over window

	// create display event to redraw window contents if needed (and not handled in the timer callback)
	// glutPostRedisplay();
}

// -----------------------  Timer ---------------------------------

/**
 * \brief Callback responsible for the scene update.
 */
void timerCb(int)
{
#ifndef SKELETON // @task_1_0
	const glm::mat4 sceneRootMatrix = glm::mat4(1.0f);

	//float elapsedTime = 0.001f * static_cast<float>(glutGet(GLUT_ELAPSED_TIME)); // milliseconds => seconds
	float time = glutGet(GLUT_ELAPSED_TIME);
	float deltaTime = (time - gameState.last_update) / 1000;
	gameState.last_update = time;
	move_player(deltaTime);
	
	// update the application state
	for (ObjectInstance* object : objects) {   // for (auto object : objects) {
		if (object != nullptr)
			object->update(deltaTime, &sceneRootMatrix);
	}
	if (gameState.keyMap[KEY_SPACE] == true)
	{
		gameState.launchMissile = true;
		shooting(objects, gameState.elapsedTime);
	}
#endif // task_1_0

	// and plan a new event
	glutTimerFunc(33, timerCb, 0); //how many ms to react??

	// create display event
	glutPostRedisplay();
}


// -----------------------  Application ---------------------------------

/**
 * \brief Initialize application data and OpenGL stuff.
 */
void initApplication() {
	// init OpenGL
	// - all programs (shaders), buffers, textures, ...
	loadShaderPrograms();
	CHECK_GL_ERROR();
	//objects.push_back(new Triangle(&commonShaderProgram));
	// objects.push_back(new SingleMesh(&commonShaderProgram));
	//objects.push_back(new Tree(&commonShaderProgram));

	//gameState.fire = new Fire(&commonShaderProgram, &fireShaderProgram);
	gameState.fire2 = new Fire2(&commonShaderProgram, &fireShaderProgram);
	gameState.skybox = new Skybox(&skyboxShaderProgram);
	//gameState.missile = new Missile(&commonShaderProgram, &missileShaderProgram);
	objects.push_back(gameState.skybox);
	objects.push_back(new House(&commonShaderProgram));
	objects.push_back(new Ground(&commonShaderProgram));
	objects.push_back(new Cat(&commonShaderProgram));
	objects.push_back(gameState.fire2);
	objects.push_back(new Turtle(&commonShaderProgram));
	//objects.push_back(gameState.fire);
	//objects.push_back(gameState.missile);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// init your Application
	// - setup the initial application state

	// player_init
	gameState.player_position = CAM_INIT_PLAYER;
	gameState.player_direction = CAM_INIT_PLAYER;
	gameState.cameraElevationAngle = 0.0f;
	gameState.cameraRotationAngle = 135.0f;

	//initiali night environment
	gameState.sunOn = false;
	gameState.reflectorOn = true;

	// set initial fog color to black
	glUseProgram(commonShaderProgram.program);
	glUniform3f(commonShaderProgram.locations.fogColor, 0.0f, 0.0f, 0.0f);
	glUseProgram(0);

	//mouse
	glutWarpPointer(gameState.windowWidth / 2, gameState.windowHeight / 2);


}

/**
 * \brief Delete all OpenGL objects and application data.
 */
void finalizeApplication(void) {

	// cleanUpObjects();

	// delete buffers
	// cleanupModels();

	// delete shaders
	cleanupShaderPrograms();
}


/**
 * \brief Entry point of the application.
 * \param argc number of command line arguments
 * \param argv array with argument strings
 * \return 0 if OK, <> elsewhere
 */
int main(int argc, char** argv) {

	// initialize the GLUT library (windowing system)
	glutInit(&argc, argv);

	glutInitContextVersion(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	 // how do we store the buffer
	// color buffer is stored in glut_rgb
	// glut_double defines how we draws the animation
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); //depth buffer display how far the obj from the camera

	// for each window
	{
		//   initial window size + title
		glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT); // specify size. size of DEPTH buffer = size window, RGB buffer has same resolution as the window
		glutCreateWindow(WINDOW_TITLE);  // specify title of window

		// callbacks - use only those you need
		glutDisplayFunc(displayCb);
		glutReshapeFunc(reshapeCb);
		glutKeyboardFunc(keyboardCb); //f1,f2, page up, page down
		glutKeyboardUpFunc(keyboardUpCb);
		glutSpecialFunc(specialKeyboardCb);     // key pressed
		glutSpecialUpFunc(specialKeyboardUpCb); // key released
		glutMouseFunc(mouseCb);
		glutMotionFunc(mouseMotionCb);
		glutTimerFunc(33, timerCb, 0);
//#ifndef SKELETON // @task_1_0
//		glutTimerFunc(33, timerCb, 0);
//#else
//		// glutTimerFunc(33, timerCb, 0);
//#endif // task_1_0

	}
	// end for each window 

	// initialize pgr-framework (GL, DevIl, etc.)
	if (!pgr::initialize(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR))
		pgr::dieWithError("pgr init failed, required OpenGL not supported?");

	// init your stuff - shaders & program, buffers, locations, state of the application
	initApplication();

	// handle window close by the user
	glutCloseFunc(finalizeApplication);

	// Infinite loop handling the events
	glutMainLoop();

	// code after glutLeaveMainLoop()
	// cleanup

	return EXIT_SUCCESS;
}
