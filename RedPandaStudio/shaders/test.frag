#version 450 core

precision mediump float;	// Precisión media

varying vec4 v_Color;		// in: color del vertex shader

in vec2 UV_Coordinates; //UV del vertex shader

//flat in int GIsEdge;

uniform sampler2D sampler;
uniform bool textActive;

out vec4 FragColor;

void main()
{
  /*
  if(GIsEdge == 0)
  {
    */
   if(textActive)
    {
      FragColor = texture(sampler, UV_Coordinates) * v_Color;
      
      float levels = 6.0;

      FragColor.x = floor(FragColor.x * levels) / levels;
      FragColor.y = floor(FragColor.y * levels) / levels;
      FragColor.z = floor(FragColor.z * levels) / levels;
      FragColor.w = 1.0;
    }
    else
    {
      FragColor = v_Color;
    }
  /*}
  else
  {
    FragColor = vec4(0.0, 0.0, 0.0, 1.0);
  }*/
}