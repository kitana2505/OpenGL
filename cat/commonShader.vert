#version 140


struct Material {  
  vec3  ambient; 
  vec3  diffuse;
  vec3  specular;
  float shininess;
  bool  useTexture;
};

uniform sampler2D texSampler;

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

in vec3 position; 
in vec3 normal;  
in vec2 texCoord; 

uniform Material material;

uniform mat4 PVMmatrix; 
uniform mat4 Vmatrix;   
uniform mat4 Mmatrix;   
uniform mat4 normalMatrix;

uniform vec3 reflectorPosition; 
uniform vec3 reflectorDirection; 
uniform int reflectorOn;
uniform int sunOn;
uniform float sunStrength;
uniform int explodeOn;
uniform float explodeStrength;
uniform float explodeTime;

uniform vec3 firePosition;
uniform float fireStrength;
uniform float fireFallof;
uniform vec3 fireDiffuse;
uniform vec3 fireAmbient;
uniform vec3 fireSpecular;
uniform vec3 fogColor;

const float density=0.0007;

smooth out vec2 texCoord_v;
smooth out vec4 color_v;
//smooth out float visibility;
//smooth out vec4 fogColor_v;

smooth out vec3 vertexPosition;
smooth out vec3 vertexNormal;

/*vec4 spotLight(Light light, Material material, vec3 vertexPosition, vec3 vertexNormal) {

  vec3 ret = vec3(0.0);

  float distance=distance(light.position, vertexPosition);
  vec3 L = normalize(light.position-vertexPosition);
  vec3 R = reflect(-L, vertexNormal);
  vec3 V = normalize(-vertexPosition);
  float ndot=dot(light.spotDirection,-L);
  float spot=pow(max(ndot,0),light.spotExponent);
  if(ndot>cos(light.spotCosCutOff)){
		ret += max(dot(L,vertexNormal),0)*material.diffuse * light.diffuse*spot;//diffuse
		ret += max(dot(R, V),0)*material.specular * light.specular*spot; //specular
		ret*=light.strength/pow(distance, 0.2);;/// 
		
	}

  return vec4(ret, 1.0);
}

vec4 fireLight(mat4 VMatrix, Material material, vec3 vertexPosition, vec3 vertexNormal) {
  vec3 ret = vec3(0.0);
  float distance=distance(firePosition, vertexPosition);
  vec3 L = normalize(firePosition - vertexPosition);
  vec3 R = reflect(-L, vertexNormal);
  vec3 V = normalize(-vertexPosition);

  ret += max(dot(L,vertexNormal),0)*material.diffuse * fireDiffuse;//diffuse
  ret += max(dot(R, V),0)*material.specular * fireSpecular; //specular

  ret*= fireStrength;
  ret/=(pow(distance, fireFallof));
  

  return vec4(ret, 1.0);
}

//sun light
vec4 directionalLight(Light light, Material material, vec3 vertexPosition, vec3 vertexNormal) {

  vec3 ret = vec3(0.0);

  vec3 L = normalize(light.position);
  vec3 R = reflect(-L, vertexNormal);
  vec3 V = normalize(-vertexPosition);
  ret += max(dot(L,vertexNormal),0)*material.diffuse * light.diffuse;//diffuse
  ret += max(dot(R, V),0)*material.specular * light.specular; //specular
  ret*=light.strength;
  return vec4(ret, 1.0);
}

Light sun;
Light flashlight;

void setupLights() {

  sun.ambient  = vec3(0.0);
  sun.diffuse  = vec3(1.0, 1.0, 1.0f);
  sun.specular = vec3(0.1f);
  //sun.strength = 1.0f;
  sun.strength = sunStrength;
  sun.position = (Vmatrix*vec4(vec3(100,100,0),0)).xyz;

  flashlight.ambient       = vec3(0.5f);
  flashlight.diffuse       = vec3(1.0);
  flashlight.specular      = vec3(0.1f);
  flashlight.spotCosCutOff = 0.3f;
  flashlight.spotExponent  = 50;
  flashlight.strength = 5;

  flashlight.position = (Vmatrix * vec4(reflectorPosition,1.0)).xyz;
  flashlight.spotDirection = normalize( (Vmatrix * vec4(reflectorDirection,0.0)).xyz);

}
*/

void main() {
	//setupLights();



  vertexPosition = (Vmatrix * Mmatrix * vec4(position, 1.0)).xyz;         
  vertexNormal   = normalize( (Vmatrix * normalMatrix * vec4(normal, 0.0) ).xyz); 




  

  //vec3 vertexPosition = (Vmatrix * Mmatrix * vec4(position, 1.0)).xyz;         // vertex in eye coordinates
  //vec3 vertexNormal   = normalize( (Vmatrix * normalMatrix * vec4(normal, 0.0) ).xyz);   // normal in eye coordinates by NormalMatrix

  vec3 globalAmbientLight = vec3(0.05f);
  vec4 outputColor = vec4(material.diffuse * globalAmbientLight, 0.0);
  //vec4 outputColor = vec4(0.5,0.5,0, 1.0);

 /* if(sunOn==1){
   outputColor += directionalLight(sun, material, vertexPosition, vertexNormal);
  }

 if(reflectorOn==0){
	outputColor += spotLight(flashlight, material, vertexPosition, vertexNormal);
 }
  
  outputColor += fireLight(Vmatrix, material, vertexPosition, vertexNormal);

  gl_Position = PVMmatrix * vec4(position, 1);   // out:v vertex in clip coordinates
  float distance = pow(distance(flashlight.position, vertexPosition), 2);
  visibility=clamp(1/(distance*density), 0.0f, 1.0f);
  fogColor_v=vec4(fogColor, 1.0f);*/



  gl_Position = PVMmatrix * vec4(position, 1);
  color_v = outputColor;
  texCoord_v = texCoord;
}
