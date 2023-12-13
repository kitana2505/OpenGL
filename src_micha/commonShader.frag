#version 140

struct Material {           // structure that describes currently used material
  vec3  ambient;            // ambient component
  vec3  diffuse;            // diffuse component
  vec3  specular;           // specular component
  float shininess;          // sharpness of specular reflection

  bool  useTexture;         // defines whether the texture is used or not
};

uniform sampler2D texSampler;  // sampler for the texture access

uniform Material material;     // current material

smooth in vec4 color_v;        // incoming fragment color (includes lighting)
smooth in vec2 texCoord_v;     // fragment texture coordinates
smooth in vec4 fogColor_v;
out vec4       color_f;        // outgoing fragment color
in float visibility;

void main() {
  if(visibility<=0){
    discard;
  }
  

  // if material has a texture -> apply it
  if(material.useTexture){
    color_f =  mix(fogColor_v, color_v * texture(texSampler, texCoord_v), visibility);
  }else{
    color_f = mix(fogColor_v, color_v, visibility);
  }
}
