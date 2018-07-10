#version 450 core

precision mediump float;	// Precisión media

vec4 v_Color;

in vec2 UV_Coordinates; //UV del vertex shader

in vec3 P;
in vec3 N;
uniform vec3 CamPos;

uniform mat4 ViewMatrix;

in mat4 modelViewMatrix;

//Tangent-Bitangent-Normal Matrix used to convert from tangent space to world space.
in mat3 TBN;

vec4 P2;
vec3 V = vec3(0.0, 0.0, 0.0);
vec3 normal = vec3(0.0, 0.0, 0.0);

//Light uniform and variables
const int maxLights = 25;
uniform int numLights;
uniform int numSpotLights;


//Light values
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

uniform float ambient = 0.4;


struct Material {
    vec3 kd;
    vec3 ka;
    vec3 ks;
    float ns;
};
uniform Material material;

//Texture uniforms
uniform sampler2D colorTexture;
uniform bool textActive;

//Texture normal uniforms
uniform sampler2D normalTexture;
uniform bool normalActive;

uniform bool silhouette;

out vec4 FragColor;


//================================
//SHADOW MAPPING variables
vec4 lightPos = light[0].position;

in vec4 FragLightPos;

uniform sampler2D shadowMap;

//Visibility limiter used for the shadow mapping
float calculateVisibility()
{
    float bias =  0.23;

    //Coordinates normalized from the light point of view (in the range of [0,1])
    vec3 projectionCoordinates = (FragLightPos.xyz / FragLightPos.w) * 0.5 + 0.5;

    //Get the visibility value depending on what you see first from the light's perspective and compare it with the value in the depth
    float visibility = (projectionCoordinates.z - bias >  texture(shadowMap, projectionCoordinates.xz).r ) ? 0.5f : 1.0f;

    return visibility;
}
//================================

void calculatePointLights()
{
    float visibility = calculateVisibility();
    for(int i = 0; i < numLights && i < maxLights; i++)
    {

        float diffuse = 0.0;
        float specular = 0.0;

        vec4 LightPos = ViewMatrix * light[i].position;

	    float d = length(LightPos.xyz - P);			        // distancia de la luz
	    vec3  L = normalize(LightPos.xyz - P);			    // Vector Luz

        diffuse = max(dot(normal, L), 0.0);		            // Cálculo de la int. difusa
        // Cálculo de la atenuación
        float attenuation = 80.0/(0.25+(0.1*d)+(0.005*d*d));
        diffuse = diffuse * attenuation;

        vec3 mid = normalize(V + L);

        
        specular = pow(max(0.0, dot(reflect(-L, normal), mid)), material.ns);


        v_Color += vec4(light[i].intensity * diffuse) * vec4(material.kd, 1.0);

        if(specular > 0.0)
        {
            //v_Color += vec4(specular * light[i].intensity) * vec4(material.ks, 1.0);
        }

    }
    
    //v_Color *= visibility;
}

void calculateSpotLights()
{

    for(int i = 0; i < numSpotLights && i < maxLights; i++)
    {
        float diffuse = 0.0;

        vec4 LightPos = normalize(ViewMatrix * spotlight[i].light.position);
        vec4 focus = ViewMatrix * vec4(spotlight[i].direction, 1.0);
        vec3  L = normalize(P - LightPos.xyz);
        float spotFactor = dot(L, normalize(focus.xyz - LightPos.xyz));

        if(1-spotFactor  < 0.01)
        {
            float d = length(LightPos.xyz - P);			        // distancia de la luz
            L = normalize(LightPos.xyz - P);			    // Vector Luz

            diffuse = max(dot(normalize(normal), L), 0.0);		            // Cálculo de la int. difusa
            // Cálculo de la atenuación
            float attenuation = 80.0/(0.25+(0.1*d)+(0.005*d*d));
            diffuse = diffuse * attenuation;

            vec3 mid = normalize(V + L);

            float specular = 2 * attenuation * pow(max(0.0, dot(reflect(-L, normal), mid)), material.ns);

            v_Color += vec4(spotlight[i].light.intensity * diffuse) * vec4(material.kd, 1.0);

            if(specular > 0)
            {
                v_Color += vec4(specular) * vec4(material.ks, 1.0);
            }
        }
    }
}


void main()
{
    if(silhouette)
    {
        FragColor = vec4(0.0, 0.0, 0.0, 0.0);
    }
    else
    {
        v_Color = vec4(0.0, 0.0, 0.0, 0.0);

        normal = N;

        //The normal mapping has been based completely in the tutorials provided at learnopengl.com
        //Specifically, the tutorial is the following: https://learnopengl.com/Advanced-Lighting/Normal-Mapping
        if(normalActive)
        {
            normal = texture(normalTexture, UV_Coordinates).rgb;
            normal = normalize(normal * 2.0 - 1.0);
            normal = normalize(TBN * normal);
        }

        V = normalize(CamPos - P);


        calculatePointLights();
        calculateSpotLights();

        v_Color += vec4(1.0, 1.0, 1.0, 1.0) * (ambient) * vec4(material.ka, 1.0);

        if(textActive)
        {
            FragColor = texture(colorTexture, UV_Coordinates) * v_Color;
            float levels = 8.0;

            FragColor.x = floor(FragColor.x * levels) / levels;
            FragColor.y = floor(FragColor.y * levels) / levels;
            FragColor.z = floor(FragColor.z * levels) / levels;
        
        }
        else
        {
            FragColor = v_Color;
        }       

        //Coordinates normalized from the light point of view (in the range of [0,1])
        //vec3 projectionCoordinates = (FragLightPos.xyz / FragLightPos.w) * 0.5 + 0.5;

        //Get the visibility value depending on what you see first from the light's perspective and compare it with the value in the depth
        //float visibility = (projectionCoordinates.z >  texture(shadowMap, projectionCoordinates.xy).r ) ? 0.5f : 1.0f;

       // FragColor = vec4(projectionCoordinates.x, projectionCoordinates.y, projectionCoordinates.z, 1.0);
        
        //FragColor = texture(shadowMap, projectionCoordinates.xy);

        //FragColor = vec4(visibility, visibility, visibility, 1.0);
    }
}
