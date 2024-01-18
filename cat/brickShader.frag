#version 140
uniform float time;           // used for simulation of moving lights (such as sun) and to select proper animation frame
uniform mat4 Vmatrix;         // view (camera) transform --> world to eye coordinates
uniform sampler2D texSampler; // sampler for texture access //brickTex

smooth in vec3 position_v;    // camera space fragment position
smooth in vec2 texCoord_v;    // fragment texture coordinates

out vec4 color_f;             // outgoing fragment color
//uniform sampler2D brickTex; // brick texture sampler – texture unit 0
uniform sampler2D mossTex; // moss texture sampler – texture unit 1


// fragment shader

//in vec2 texCoords_v; // texture coordinates

void main() {
	vec4 brickTexColor = texture(texSampler, texCoord_v); // sample textures
	vec4 mossTexColor = texture(mossTex, texCoord_v);
	// mix textures together – mixing ratio is given by moss texture alpha component
	vec4 texColor = mix(brickTexColor, mossTexColor, mossTexColor.a*0.25f);
	//vec4 texColor = mix(brickTexColor, mossTexColor, 0.5);

	// compute lighting → color (vec3)
	
	// compute final fragment color
	//color_f = mix(texColor, texColor, 0.65f);
	color_f = texColor;//, texColor, 0.65f);
	//color_f = vec4(1.0f,1.0f,1.0f,1.0f);
}