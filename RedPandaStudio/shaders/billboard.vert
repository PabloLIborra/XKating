#version 450 core

layout (location = 0) in vec3 VPosition;

//The technique used for developing the billboards in our engine is based in the tutorials provided by ogldev.atspace.co.uk
//More precisely, it's based on this tutorial: http://ogldev.atspace.co.uk/www/tutorial27/tutorial27.html
//This also includes our shaders, used for rendering the billboards
void main()
{
    gl_Position = vec4(VPosition, 1.0);
}