#version 140
uniform float time;           // used for simulation of moving lights (such as sun) and to select proper animation frame
uniform mat4 Vmatrix;         // view (camera) transform --> world to eye coordinates
uniform sampler2D texSampler; // sampler for texture access

smooth in vec3 position_v;    // camera space fragment position
smooth in vec2 texCoord_v;    // fragment texture coordinates

out vec4 color_f;             // outgoing fragment color

uniform ivec2 pattern = ivec2(3,2);

uniform float frameDuration = 0.001f;


vec4 sampleTexture(int frame) {
	int x_size=3;
	int y_size=2;
  vec2 size=vec2(1/pattern.x, 1/pattern.y);
  vec2 coord=texCoord_v;
  int xframe=frame%(x_size);
  //int yframe=int(frame/x_size);
  int yframe=1;
  coord.x=(texCoord_v.x+xframe)/pattern.x;
  coord.y=(texCoord_v.y+yframe)/pattern.y;
  return texture(texSampler, coord);
}

void main() {
  // frame of the texture to be used for explosion drawing 
  int frame = int(time / frameDuration);

  // sample proper frame of the texture to get a fragment color  
  color_f = sampleTexture(frame);
}
