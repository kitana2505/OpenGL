//----------------------------------------------------------------------------------------
/**
 * \file    data.h
 * \author  Michal Vonášek
 * \date    2023
 * \brief   Basic defines and data structures.
 */
//----------------------------------------------------------------------------------------

#ifndef __DATA_H
#define __DATA_H

#define WINDOW_WIDTH   750
#define WINDOW_HEIGHT  750
#define WINDOW_TITLE   "CAT"

/// keys used in the key map
enum { FORWARD, LEFT, BACKWARD, RIGHT, RUN, KEYS_COUNT, KEY_SPACE};

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

//turtle
#define TURTLE_MODEL  "data/airplane/11803_Airplane_v1_l1.obj"
#define TURTLE_SCALE 2.0f
#define TURTLE_INITIAL_POS glm::vec3(-5.0f,8.0f, 15.0f)
#define TURTLE_ROTATION 90.0f
#define TURTLE_SPEED    0.5f

//missile
#define MISSILE_MAX_DISTANCE       1.5f
#define MISSILE_LAUNCH_TIME_DELAY  0.25f // seconds
#define MISSILE_SIZE     0.0085f
#define MISSILE_SPEED              1.5f


//house
#define HOUSE_MODEL "data/house/house_blue/house.obj"
#define HOUSE_SCALE 2.0f
#define HOUSE_TRANSLATE glm::vec3(5.0f,0.8f,0)

//trees
#define TREE_SPACING 10.0f
//#define TREE_MODEL "data/Spruce_obj/Spruce.obj"
#define TREE_MODEL "data/tree/Tree.obj"
#define TREE_SCALE 2.0f

//grass
#define GRASS_MODEL "data/trava/trava.obj"
#define GRASS_SPACING 20.0f
#define GRASS_ROTATION_VEC glm::vec3(3,0,2)
#define GRASS_ROTATION_size -0.5f
#define GRASS_TRASLATION glm::vec3(0, -0.5f, 0)
#define GRASS_SCALE 1.0f

//ground
#define GROUND_MODEL "data/grass/grass.obj"
#define GROUND_SCALE glm::vec3(10.0f, 0.1f, 10.0f)
#define GROUND_ROTATION -1.6f
#define GROUND_TRANSLATION glm::vec3(0,-1.0f,0)

//bush
#define BUSH_MODEL "data/bush/bush.obj"
#define BUSH_SPACING 30.0f
#define BUSH_ROTATION_VEC glm::vec3(1,1,1)
#define BUSH_ROTATION_size 0.0f
#define BUSH_TRASLATION glm::vec3(0, -0.5f, 0)
#define BUSH_SCALE 1.0f

//firewood
#define WOOD_MODEL "data/fire_wood/fireplace_wood.obj"
#define WOOD_SPACING 15.0f
#define WOOD_ROTATION_VEC glm::vec3(1,1,1)
#define WOOD_ROTATION_size 0.0f
#define WOOD_TRASLATION glm::vec3(0, -0.5f, 0)
#define WOOD_SCALE 0.5f
#define WOOD_APPEARANCE_CHANCE 0.2f

//fern
#define FERN_MODEL "data/kapradi/kapradi.obj"
#define FERN_SPACING 35.0f
#define FERN_ROTATION_VEC glm::vec3(1,1,1)
#define FERN_ROTATION_size 0.0f
#define FERN_TRASLATION glm::vec3(0, 0, 0)
#define FERN_SCALE 0.5f

//plant
#define PLANT_MODEL "data/plant/BushObj.obj"
#define PLANT_SPACING 20.0f
#define PLANT_ROTATION_VEC glm::vec3(1,1,1)
#define PLANT_ROTATION_size 0.0f
#define PLANT_TRASLATION glm::vec3(0, -0.9f, 0)
#define PLANT_SCALE 0.2f

//fire
/// size of fire for moving purposes
#define FIRE_SIZE 1.0f
/// Light strengths
#define SMALL_FIRE_STRENGTH 15.0f
#define MEDIUM_FIRE_STRENGTH 17.0f
#define HIGH_FIRE_STRENGTH 20.0f
#define BURNING_TIME 5.0f
#define FIRE_SCALE glm::vec3(0.9f,0.5f, 0.9f)
#define FIRE_TRANSLATE glm::vec3(0,0.5f,0)
#define FIRE_BURN_MULT 0.2f
/// wood count thresholds for higher fires
#define HIGH_FIRE_THRESHOLD 5
#define MEDIUM_FIRE_THRESHOLD 2
const float flame_sizes[3] = { 0.5f, 1.0f, 1.5f };
#define EXPLOSION_TEXTURE_NAME "data/flames/fire2.jpg"

//fire big
#define FIRE_BIG_SCALE  glm::vec3(0.125f, 0.20f, 0.25f)
#define FIRE_BIG_TRANSLATE  glm::vec3(-8.5, 4, 0)
#define FIRE_ROTATE  glm::vec3(0, 0, 1)

//fire medium
#define FIRE_MEDIUM_SCALE  glm::vec3(0.10f, 0.15f, 0.20f)
#define FIRE_MEDIUM_TRANSLATE  glm::vec3(12, 2, 0)

//fire small
#define FIRE_SMALL_SCALE  glm::vec3(0.075f, 0.10f, 0.15f)
#define FIRE_SMALL_TRANSLATE  glm::vec3(2, 2, 0)

// player config 
#define PLAYER_WALKING_SPEED  2.0f
#define PLAYER_RUNNING_SPEED  4.5f
#define PLAYER_HEIGHT  0.5f
#define CAM_INIT_PLAYER glm::vec3(6.69, 1.0f, -19.63f)

//camera
#define CAMERA_COUNT 4;
#define STATIC_CAMERA_1 glm::vec3(-80,15,20)
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
