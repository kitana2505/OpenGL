#version 140
out vec4 fragmentColor;

// declare winWidth and alpha uniforms 
// winWidth will be int and it will store window width in pixels 
// alpha will be float and it will store an animation parameter in the interval [0, 1] 


// define the input variable color 
in vec4 color;

uniform mat4  PVMmatrix;   // transformation matrix


void main()
{
    fragmentColor = color;
  
}
