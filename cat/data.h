//----------------------------------------------------------------------------------------
/**
 * \file    data.h
 * \author 
 * \date    2024
 * \brief   Basic defines 
 */
//----------------------------------------------------------------------------------------

#ifndef __DATA_H
#define __DATA_H

#define WINDOW_WIDTH   750
#define WINDOW_HEIGHT  750
#define WINDOW_TITLE   "CAT"

/// keys used in the key map
enum { FORWARD, LEFT, BACKWARD, RIGHT, RUN, KEY_SPACE, KEYS_COUNT};

#define SCENE_WIDTH  100.0f
#define SCENE_HEIGHT 100.0f
#define SCENE_DEPTH  100.0f

#define CAMERA_ELEVATION_MAX 70.0f
#define CAMERA_ROTAION_MAX 90.0f

//cat
#define CAT_MODEL  "data/animal_cat/samy.obj"
#define CAT_SCALE  0.5f
#define CAT_INITIAL_POS glm::vec3(0, 0.9, -10)
#define CAT_ROTATION 0.0f
#define CAT_TEXTURE "data/animal_cat/texture_cat.jpg"

//banner

#define BANNER_TEXTURE_NAME  "data/banner/scrollrain.jpg"
#define BANNER_SIZE      1.0f

//Rabbit
#define RABBIT_MODEL  "data/animal_rabbit/rabbit.obj"
#define RABBIT_SCALE  0.3f
#define RABBIT_INITIAL_POS glm::vec3(20.0, 0.9, -20)
#define RABBIT_ROTATION 0.0f
#define RABBIT_TEXTURE "data/animal_rabbit/texture_rabbit.jpg"


//AIRPLANE
#define AIRPLANE_MODEL  "data/airplane/11803_Airplane_v1_l1.obj"
#define AIRPLANE_SCALE 2.0f
#define AIRPLANE_INITIAL_POS glm::vec3(-5.0f,8.0f, 15.0f)
#define AIRPLANE_ROTATION 90.0f
#define AIRPLANE_SPEED    0.5f
#define AIRPLANE_TEXTURE "data/airplane/texture_airplane.jpg"


//missile
#define MISSILE_MAX_DISTANCE       3.5f
#define MISSILE_LAUNCH_TIME_DELAY  0.25 // seconds
#define MISSILE_SIZE     0.25f
#define MISSILE_SPEED              2.5f

//pole
#define POLE_SIZE     1.0f
#define POLE_ROT_FREQ    1      
// 
//brick
#define BRICK_MODEL "data/brick/brick.obj"
#define BRICK_SCALE  3.0f
#define BRICK_INITIAL_POS glm::vec3(4.1f,0.0f,-3.0f)
#define BRICK_ROTATION 0.0f

//house
#define HOUSE_MODEL "data/house/house_blue/house.obj"
#define HOUSE_SCALE 2.0f
#define HOUSE_TRANSLATE glm::vec3(5.0f,0.8f,0)
#define HOUSE_TEXTURE "data/house/house_blue/texture_house.jpg"

//trees
#define TREE_SPACING 10.0f
#define TREE_MODEL "data/tree/Tree.obj"
#define TREE_SCALE 2.0f
#define TREE_LOCATION glm::vec3(2.0, 0.9,3.0)


//ground
#define GROUND_MODEL "data/grass/grass.obj"
#define GROUND_SCALE glm::vec3(10.0f, 0.1f, 10.0f)
#define GROUND_ROTATION -1.6f
#define GROUND_TRANSLATION glm::vec3(0,-1.0f,0)


//fire
#define FIRE_MODEL_NAME2  "data/low_poly_fireplace/fireplace.obj"
#define FIRE_SCALE glm::vec3(0.9f,0.5f, 0.9f)
#define FIRE_TRANSLATE glm::vec3(0,0.5f,0)
#define FIRE_SIZE 1.0f
#define FIRE_TEXTURE_NAME "data/flames/fire2.jpg"


// player config 

#define WALKING_SPEED  3.0f
#define HIGH_SPEED  8.0f

#define PLAYER_HEIGHT  0.5f
#define CAM_INIT_PLAYER glm::vec3(6.69, 1.0f, -19.63f)

// explosion size
#define EXPLOSION_SIZE 2.5f;
#define EXPLOSION_TEXTURE_NAME "data/explosion/explode.png"

//camera
#define CAMERA_COUNT 4;
//#define STATIC_CAMERA_1 glm::vec3(-80,15,20)
#define STATIC_CAMERA_2 glm::vec3(17.0f,30.0f,-12.0f)

//Skybox
#define SKYBOX_DAY_TEXTURE_NAME "data/skybox/miramar"
#define SKYBOX_NIGHT_TEXTURE_NAME "data/skybox/skybox"

const int flameNumQuadVertices = 4;
/// <summary>
/// flame vertex and texture coordinates
/// </summary>
const float flameVertexData[flameNumQuadVertices * 5] = {

	  -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	   1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	  -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
	   1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
};


//
// "game over" banner geometry definition 
//

const int bannerNumQuadVertices = 4;
const float bannerVertexData[bannerNumQuadVertices * 5] = {

   // x      y      z     u     v
0.0f,  1.0f, 0.0f, 0.0f, 1.0f, //coord of window, coord of texture
0.0f,  0.0f, 0.0f, 0.0f, 0.0f,
 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
 1.0f, 0.0f, 0.0f, 1.0f, 0.0f
};

const int explosionNumQuadVertices = 4;
const float explosionVertexData[explosionNumQuadVertices * 5] = {

	// x      y     z     u     v
	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
	 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
};

// default shaders - color per vertex and matrix multiplication
const std::string colorVertexShaderSrc(
    "#version 140\n"
    "uniform mat4 PVMmatrix;\n"
    "in vec3 position;\n"
    "in vec3 color;\n"
    "smooth out vec4 theColor;\n"
    "void main() {\n"
    "  gl_Position = PVMmatrix * vec4(position, 1.0);\n"
    "  theColor = vec4(color, 1.0);\n"
    "}\n"
);

const std::string colorFragmentShaderSrc(
    "#version 140\n"
    "smooth in vec4 theColor;\n"
    "out vec4 outputColor;\n"
    "void main() {\n"
    "  outputColor = theColor;\n"
    "}\n"
);

// each vertex shader receives screen space coordinates and calculates world direction
const std::string skyboxFarPlaneVertexShaderSrc(
    "#version 140\n"
    "\n"
    "uniform mat4 inversePVmatrix;\n"
    "in vec2 screenCoord;\n"
    "out vec3 texCoord_v;\n"
    "\n"
    "void main() {\n"
    "  vec4 farplaneCoord = vec4(screenCoord, 0.9999, 1.0);\n"
    "  vec4 worldViewCoord = inversePVmatrix * farplaneCoord;\n"
    "  texCoord_v = worldViewCoord.xyz / worldViewCoord.w;\n"
    "  gl_Position = farplaneCoord;\n"
    "}\n"
);

// fragment shader uses interpolated 3D tex coords to sample cube map
const std::string skyboxFarPlaneFragmentShaderSrc(
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

#endif // __DATA_H
