#version 410 core

precision mediump float;	// Precisión media

in vec4 v_Color;		// in: color del vertex shader
in vec2 UV_Coordinates; //UV del vertex shader

uniform sampler2D sampler;
uniform bool textActive;

out vec4 FragColor;

void main()
{
  if(textActive)
  {
    FragColor = texture(sampler, UV_Coordinates) * v_Color;
  }
  else
  {
    FragColor = v_Color;
  }
}