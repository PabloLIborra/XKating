#include "TResourceShader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

//Loads the resource
bool TResourceShader::loadResource()
{
    std::string shaderCode;
    GLint Result = GL_FALSE;
	int InfoLogLength;

    //In first instance we create the shader of the type defined
    shaderID = glCreateShader(shaderType);
    
    //We read completely the shader
    std::ifstream shaderStream(name, std::ios::in);
    if(shaderStream.is_open())
    {
        std::stringstream sstr;
		sstr << shaderStream.rdbuf();
        shaderCode = sstr.str();
        shaderStream.close();
    }
    else
    {
		printf("Couldn't open %s\n", name);
        return false;
	}

    //Compile the shader in OpenGL
    printf("Compiling shader : %s\n", name);
    char const * sourcePointer = shaderCode.c_str();
    glShaderSource(shaderID, 1, &sourcePointer, NULL);
    glCompileShader(shaderID);

    //Check for errors in the compile process
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if(InfoLogLength > 0)
    {
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(shaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
    }

    return true;
}

//Draw method. It's only here because this class is a child from TResource, and need to implement it
void TResourceShader::draw ()
{
    
}