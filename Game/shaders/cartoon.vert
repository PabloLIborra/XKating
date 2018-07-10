/*
#version 450 core

layout(location = 0) in vec4 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 UV;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

mat4 modelViewMatrix;
mat4 normalMatrix4;
mat3 normalMatrix;

vec4 aux;

varying vec4 v_Color;
varying vec2 UV_Coordinates;

out vec3 VNormal;
out vec3 VPosition;

const int maxLights = 15;
uniform int numLights;

struct Light {
   vec4 position;
   vec4 intensity;
};
uniform Light light[maxLights];

struct Material {
    vec3 kd;
    vec3 ka;
    vec3 ks;
    float ns;
};
uniform Material material;

void main()
{
    float ambient = 1;                               

    v_Color = vec4(0.0, 0.0, 0.0, 1.0);

    modelViewMatrix = ViewMatrix * ModelMatrix;

    normalMatrix4 = transpose(inverse(modelViewMatrix));

    normalMatrix = mat3(normalMatrix4[0][0], normalMatrix4[0][1], normalMatrix4[0][2], normalMatrix4[1][0], normalMatrix4[1][1], normalMatrix4[1][2], normalMatrix4[2][0], normalMatrix4[2][1], normalMatrix4[2][2]);

    vec3 P = vec3(modelViewMatrix * vertexPosition);	          // Posición del vértice
	vec3 N = vec3(modelViewMatrix * vec4(vertexNormal, 0.0));    // Normal del vértice

    vec4 P2 = vec4(P.x, P.y, P.z, 1.0);
    vec4 posactual = {modelViewMatrix[3][0], modelViewMatrix[3][1], modelViewMatrix[3][2], modelViewMatrix[3][0]};
    vec3 V = normalize(vec3(modelViewMatrix * posactual * (vec4(0.0, 0.0, 0.0, 1.0) - P2)));

    for(int i = 0; i < numLights && i < maxLights; i++){

        float diffuse = 0.0;

        vec4 LightPos = ViewMatrix * light[i].position;

	    float d = length(LightPos.xyz - P);			        // distancia de la luz
	    vec3  L = normalize(LightPos.xyz - P);			    // Vector Luz

        diffuse = max(dot(N, L), 0.0);		            // Cálculo de la int. difusa
        // Cálculo de la atenuación
        float attenuation = 80.0/(0.25+(0.1*d)+(0.005*d*d));
        diffuse = diffuse * attenuation;
        
        float specular = 2 * attenuation * pow(max(0.0, dot(reflect(-L, N), V)), material.ns);

        v_Color += vec4(light[i].intensity * diffuse) * vec4(material.kd, 1.0);

        if(specular > 0)
        {
            //v_Color += vec4(specular) * vec4(material.ks, 1.0);
        }

    }

    v_Color += vec4(1.0, 1.0, 1.0, 1.0) * (ambient) * vec4(material.ka, 1.0);

    VNormal = normalize(normalMatrix * vertexNormal);
    aux = modelViewMatrix * vertexPosition;
    VPosition = vec3(aux.x, aux.y, aux.z);

    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vertexPosition;

    UV_Coordinates = UV;

}
*/


#version 450 core

layout(location = 0) in vec4 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 UV;
layout(location = 3) in vec3 tangents;
layout(location = 4) in vec3 bitangents;

uniform mat4 ModelMatrix;
uniform mat4 mvMatrix;
uniform mat4 mvpMatrix;

out vec2 UV_Coordinates;
out vec3 P;
out vec3 N;

out mat4 modelViewMatrix;

out mat3 TBN;

//================================
uniform mat4 lightSpaceView;

out vec4 FragLightPos;
//================================

void main()
{
    modelViewMatrix = mvMatrix;

    P = vec3(mvMatrix * vertexPosition);	          // Posición del vértice
	N = vec3(mvMatrix * vec4(vertexNormal, 0.0));    // Normal del vértice

    //This matrix is used to convert from tangent space to world space
    //The normal mapping has been based completely in the tutorials provided at learnopengl.com
    //Specifically, the tutorial is the following: https://learnopengl.com/Advanced-Lighting/Normal-Mapping
    vec3 T = normalize(vec3(ModelMatrix * vec4(tangents, 0.0)));
    vec3 B = normalize(vec3(ModelMatrix * vec4(bitangents, 0.0)));
    vec3 norm = normalize(vec3(ModelMatrix * vec4(vertexNormal, 0.0)));
    TBN = mat3(T, B, norm);

    gl_Position = mvpMatrix * vertexPosition;

    UV_Coordinates = UV;

    //================================
    FragLightPos = lightSpaceView * ModelMatrix * vertexPosition;
    //================================
}
