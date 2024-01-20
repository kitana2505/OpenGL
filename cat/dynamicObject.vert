#version 140
in vec3 position;

uniform mat4  PVMmatrix;   // transformation matrix

// define the output variable color 
out vec4 color;

// declare the alpha uniform 
// alpha will be float and it will store an animation parameter in the interval [0, 1] 
//float alpha = 0.5f; // from the interval <0..1>
uniform float alpha;


vec3 toEllipse(vec3 position, vec3 scale,float alpha)
{
      
    vec3 ellipse = vec3(position.x * scale.x, position.y * scale.y, position.z * scale.z);
    return mix(position, ellipse,alpha);
    //return ellipse;
}

void main()
{
     
    vec3 morphPosition = toEllipse(position.xyz, vec3(5.0f, 2.0f, 2.0f), alpha);

    gl_Position = PVMmatrix * vec4(morphPosition, 1.0);

    // Set color to red
    color = vec4(0.2, 0.0, 0.8, 1.0);
}
