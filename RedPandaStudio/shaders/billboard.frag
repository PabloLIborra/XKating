#version 450 core

uniform sampler2D texture;

in vec2 TextCoord;
out vec4 FragColor;

//The technique used for developing the billboards in our engine is based in the tutorials provided by ogldev.atspace.co.uk
//More precisely, it's based on this tutorial: http://ogldev.atspace.co.uk/www/tutorial27/tutorial27.html
//This also includes our shaders, used for rendering the billboards
void main()
{

    FragColor = texture2D(texture, TextCoord);

    //We discard this kind of fragment to achieve transparency in our billboards
    if(FragColor.r == 0 && FragColor.g == 0 && FragColor.b == 0)
    {
        discard;
    }


}