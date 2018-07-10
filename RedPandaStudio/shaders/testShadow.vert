#version 450 core

layout(location = 0) in vec4 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 UV;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

mat4 modelViewMatrix;

varying vec4 v_Color;
varying vec2 UV_Coordinates;

const int maxLights = 25;
uniform int numLights;
uniform int numSpotLights;

struct Light {
   vec4 position;
   vec4 intensity;
};

uniform Light light[maxLights];

struct SpotLight
{
    Light light;
    vec3 direction;
    float cutoff;
};
uniform SpotLight spotlight[maxLights];

struct Material {
    vec3 kd;
    vec3 ka;
    vec3 ks;
    float ns;
};
uniform Material material;

//================================
uniform vec4 lightSpaceView;

varying vec4 FragPos;
varying vec4 FragLightPos;
varying vec3 Normal;

out vec4 lightPos;
out vec4 viewPos;
//================================

void main()
{
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vertexPosition;

    UV_Coordinates = UV;

    //================================
    FragPos = ModelMatrix * vertexPosition;
    FragLightPos = lightSpaceView * FragPos;
    Normal = transpose(inverse(mat3(ModelMatrix))) * vertexNormal;

    lightPos = light[0].position;
    viewPos = ViewMatrix * ModelMatrix * vertexPosition;
    //================================
}