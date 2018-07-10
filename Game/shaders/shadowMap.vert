#version 450 core
layout (location = 0) in vec4 aPos;

//Light position
/*const int maxLights = 15;
uniform int numLights;

struct Light {
   vec4 position;
   vec4 intensity;
};

uniform Light light[maxLights];*/

uniform mat4 lightProView;
uniform mat4 lightModel;

void main()
{   
    gl_Position = lightProView * lightModel * aPos;
}