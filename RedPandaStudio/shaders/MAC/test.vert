#version 410 core

layout(location = 0) in vec4 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 UV;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

mat4 modelViewMatrix;

out vec4 v_Color;
out vec2 UV_Coordinates;

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

    vec3 P = vec3(modelViewMatrix * vertexPosition);	          // Posición del vértice
	vec3 N = vec3(modelViewMatrix * vec4(vertexNormal, 0.0));    // Normal del vértice

    vec4 P2 = vec4(P.x, P.y, P.z, 1.0);
    vec4 posactual = vec4(modelViewMatrix[3][0], modelViewMatrix[3][1], modelViewMatrix[3][2], modelViewMatrix[3][0]);
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


    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vertexPosition;

    UV_Coordinates = UV;

}
