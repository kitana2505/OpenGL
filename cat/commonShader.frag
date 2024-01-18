#version 140

struct Material {           // structure that describes currently used material
  vec3  ambient;            // ambient component
  vec3  diffuse;            // diffuse component
  vec3  specular;           // specular component
  float shininess;          // sharpness of specular reflection

  bool  useTexture;         // defines whether the texture is used or not
};

struct Light {      
  vec3  ambient;      
  vec3  diffuse;      
  vec3  specular;      
  vec3  position; 
  vec3  spotDirection;
  float spotCosCutOff;
  float spotExponent; 
  float strength;
};

uniform sampler2D texSampler;  // sampler for the texture access
uniform float time;         // time used for simulation of moving lights (such as sun)
uniform Material material;     // current material

//smooth in vec4 color_v;        // incoming fragment color (includes lighting)
smooth in vec2 texCoord_v;     // fragment texture coordinates
//smooth in vec4 fogColor_v;
float fogDensity = 0.001f;
int fogOn = 1;
vec4 fogColor_f;
out vec4       color_f;        // outgoing fragment color
//in float visibility;
float visibility;
smooth in vec3 vertexPosition;
smooth in vec3 vertexNormal;

// uniform variables
uniform mat4 PVMmatrix; 
uniform mat4 Vmatrix;   
uniform mat4 Mmatrix;   
uniform mat4 normalMatrix;

uniform vec3 reflectorPosition; 
uniform vec3 reflectorDirection; 
uniform int reflectorOn;
uniform float reflectorSpotCosCutOff;
uniform int reflectorExponent;
uniform int sunOn;
uniform float sunStrength;
//uniform int explodeOn;
//uniform float explodeStrength;
//uniform float explodeTime;

uniform vec3 firePosition;
uniform float fireStrength;
//uniform float fireFallof;
//uniform vec3 fireDiffuse;
//uniform vec3 fireAmbient;
//uniform vec3 fireSpecular;
//uniform int fogOn;
uniform vec3 fogColor;

//const float density=0.0007;


vec4 spotLight(Light light, Material material, vec3 vertexPosition, vec3 vertexNormal) {

  vec3 ret = vec3(0.0);

  float distance=distance(light.position, vertexPosition);
  vec3 L = normalize(light.position-vertexPosition);
  vec3 R = reflect(-L, vertexNormal);
  vec3 V = normalize(-vertexPosition);
  float ndot=dot(light.spotDirection,-L);
  float spot=pow(max(ndot,0),light.spotExponent);
  if(ndot>cos(light.spotCosCutOff)){
		ret += material.ambient * light.ambient;
		ret += max(dot(L,vertexNormal),0)*material.diffuse * light.diffuse*spot;//diffuse
		ret += max(dot(R, V),0)*material.specular * light.specular*spot; //specular
		ret*=light.strength/pow(distance, 0.2);;/// 
		
	}

  return vec4(ret, 1.0);
}

vec4 fireLight(mat4 VMatrix, Material material, vec3 vertexPosition, vec3 vertexNormal, Light light) {
  vec3 ret = vec3(0.0);
  float distance=distance(firePosition, vertexPosition);
  vec3 L = normalize(firePosition - vertexPosition);
  vec3 R = reflect(-L, vertexNormal);
  vec3 V = normalize(-vertexPosition);
  ret += material.ambient * light.ambient;
  ret += max(dot(L,vertexNormal),0)*material.diffuse * light.diffuse;//diffuse
  ret += max(dot(R, V),0)*material.specular * light.specular; //specular

  ret*= fireStrength;
  //ret/=(pow(distance, fireFallof));
  

  return vec4(ret, 1.0);
}

//sun light
vec4 directionalLight(Light light, Material material, vec3 vertexPosition, vec3 vertexNormal) {

  vec3 ret = vec3(0.0);

  vec3 L = normalize(light.position);
  vec3 R = reflect(-L, vertexNormal);
  vec3 V = normalize(-vertexPosition);
  ret += material.ambient * light.ambient;
  ret += max(dot(L,vertexNormal),0)*material.diffuse * light.diffuse;//diffuse
  ret += max(dot(R, V),0)*material.specular * light.specular; //specular
  ret*=light.strength;
  return vec4(ret, 1.0);
}

Light sun;
Light flashlight;
Light fire;
float sunSpeed = 0.5f;

void setupLights() {

  sun.ambient  = vec3(0.0);
  sun.diffuse  = vec3(1.0, 1.0, 1.0f);
  sun.specular = vec3(0.1f);
  //sun.strength = 1.0f;
  sun.strength = sunStrength;
  sun.position = (Vmatrix * vec4(cos(time * sunSpeed)*100, 100.0, sin(time * sunSpeed)*100, 0.0)).xyz;
  //sun.position = (Vmatrix*vec4(vec3(100,100,0),0)).xyz;

  flashlight.ambient       = vec3(0.5f);
  flashlight.diffuse       = vec3(1.0);
  flashlight.specular      = vec3(0.1f);
  flashlight.spotCosCutOff = reflectorSpotCosCutOff;
  flashlight.spotExponent  = reflectorExponent;
  flashlight.strength = 5;

  flashlight.position = (Vmatrix * vec4(reflectorPosition,1.0)).xyz;
  flashlight.spotDirection = normalize( (Vmatrix * vec4(reflectorDirection,0.0)).xyz);

  fire.diffuse = vec3(1.0f, 0.4f, 0.0f);
  fire.specular = vec3(1.5f, 0.5f, 0.0f);
  fire.ambient = vec3(0.0f,0.0f,0.0f);
}

void main() {

  vec3 normal = normalize(vertexNormal);
  vec3 globalAmbientLight = vec3(0.05f);
  color_f = vec4(material.diffuse * globalAmbientLight, 0.0);

 //color_f = color_v;

  setupLights();

  if(sunOn==1){
   color_f += directionalLight(sun, material, vertexPosition, normal);
  }

 if(reflectorOn==0){
	color_f += spotLight(flashlight, material, vertexPosition, normal);
 }
  
  color_f += fireLight(Vmatrix, material, vertexPosition, normal,fire);


  float fogDist = pow(distance(flashlight.position, vertexPosition), 2);
	  //visibility = clamp(1/(distance*density), 0.0f, 1.0f);
  float fog = 0; 
		
  if(fogOn==1){
	  // can see fog from view of the player/ camera 
	  //float distance = pow(distance(flashlight.position, vertexPosition), 2);
	  //fogDensity = 0.1f;
	  fog = exp(-1 * fogDist * fogDensity);
	  fog = clamp(fog, 0.0f, 1.0f);
	  if (fog != 0) {
			//vec4 fogCol = vec4(0.8f, 0.8f, 0.8f, 1.0);
			fogColor_f = vec4(fogColor, 1.0f);
	  }
	}
  else {
		fog = 1;
		fogColor_f = vec4(fogColor, 1.0f);
		}
  //if(visibility<=0){
    //discard;
  //}
  

  // if material has a texture -> apply it
  if(material.useTexture){
    //color_f =  mix(fogColor_v, color_v * texture(texSampler, texCoord_v), visibility);
	color_f =  mix(fogColor_f, color_f * texture(texSampler, texCoord_v),fog);

  }else{
    //color_f = mix(fogColor_v, color_v, visibility);
	color_f = mix(fogColor_f, color_f,fog);
  }
}
